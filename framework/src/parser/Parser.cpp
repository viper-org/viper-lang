// Copyright 2023 solar-mist


#include "parser/Parser.h"

#include "parser/ast/expression/BinaryExpression.h"
#include "parser/ast/expression/UnaryExpression.h"
#include "parser/ast/expression/BooleanLiteral.h"
#include "parser/ast/expression/NullptrLiteral.h"
#include "parser/ast/expression/CastExpression.h"
#include "parser/ast/expression/ScopeResolution.h"
#include "parser/ast/statement/BreakStatement.h"
#include "parser/ast/statement/ContinueStatement.h"

#include "lexer/Token.h"

#include "symbol/Identifier.h"
#include "symbol/Import.h"

#include "type/PointerType.h"
#include "type/StructType.h"
#include "type/ArrayType.h"

#include <algorithm>
#include <filesystem>
#include <format>

namespace parser
{
    Parser::Parser(std::vector<lexing::Token>& tokens, diagnostic::Diagnostics& diag, symbol::ImportManager& importManager)
        : mTokens(tokens)
        , mImportManager(importManager)
        , mPosition(0)
        , mScope(nullptr)
        , mDiag(diag)
    {
    }

    lexing::Token Parser::current() const
    {
        return mTokens.at(mPosition);
    }

    lexing::Token Parser::consume()
    {
        return mTokens.at(mPosition++);
    }

    lexing::Token Parser::peek(int offset) const
    {
        return mTokens.at(mPosition + offset);
    }

    void Parser::expectToken(lexing::TokenType tokenType)
    {
        if (current().getTokenType() != tokenType)
        {
            lexing::Token temp(tokenType, {0, 0}, {0, 0});
            mDiag.compilerError(current().getStart(), current().getEnd(), std::format("expected '{}{}{}' before '{}{}{}' token",
                fmt::bold, temp.getId(), fmt::defaults, fmt::bold, current().getId(), fmt::defaults));
        }
    }

    void Parser::expectEitherToken(std::vector<lexing::TokenType> tokenTypes)
    {
        std::string tokensString;
        for (auto& tokenType : tokenTypes)
        {
            if (current().getTokenType() == tokenType)
                return;

            lexing::Token temp(tokenType, {0, 0}, {0, 0});
            tokensString += std::format("'{}{}{}', ", fmt::bold, temp.getId(), fmt::defaults);
        }

        tokensString = tokensString.substr(0, tokensString.size() - 2);

        mDiag.compilerError(current().getStart(), current().getEnd(), std::format("expected either {} before '{}{}{}' token",
            tokensString, fmt::bold, current().getId(), fmt::defaults));
    }

    std::vector<ASTNodePtr> Parser::parse()
    {
        std::vector<ASTNodePtr> result;

        while (mPosition < mTokens.size())
        {
            auto node = parseGlobal(result);
            if (node)
            {
                result.push_back(std::move(node));
            }
        }

        return result;
    }

    ASTNodePtr Parser::parseGlobal(std::vector<ASTNodePtr>& nodes)
    {
        std::vector<GlobalAttribute> attributes;
        if (current().getTokenType() == lexing::TokenType::DoubleLeftSquareBracket)
        {
            parseAttributes(attributes);
        }

        if (current().getTokenType() == lexing::TokenType::ExportKeyword)
        {
            consume();
            expectEitherToken({ lexing::TokenType::FuncKeyword, lexing::TokenType::GlobalKeyword,
                                 lexing::TokenType::StructKeyword, lexing::TokenType::UsingKeyword,
                                 lexing::TokenType::EnumKeyword });
        }

        switch (current().getTokenType())
        {
            case lexing::TokenType::FuncKeyword:
                return parseFunction(attributes);
            case lexing::TokenType::StructKeyword:
                return parseStructDeclaration();
            case lexing::TokenType::GlobalKeyword:
                return parseGlobalDeclaration();
            case lexing::TokenType::ImportKeyword:
            {
                auto symbols = parseImportStatement();
                std::move(symbols.first.begin(), symbols.first.end(), std::back_inserter(nodes));
                std::move(symbols.second.begin(), symbols.second.end(), std::back_inserter(mSymbols));
                return nullptr;
            }
            case lexing::TokenType::NamespaceKeyword:
                return parseNamespace();
            case lexing::TokenType::UsingKeyword:
                if (peek(1).getTokenType() == lexing::TokenType::StructKeyword)
                {
                    consume();
                    StructDeclarationPtr structDecl = parseStructDeclaration();
                    Type::AddAlias(structDecl->getNames(), structDecl->getType());
                    return structDecl;
                }
                return parseUsingDeclaration();
                
            case lexing::TokenType::EnumKeyword:
                return parseEnumDeclaration();
            default:
                mDiag.compilerError(current().getStart(), current().getEnd(), "Unexpected token. Expected global statement");
        }
    }

    int Parser::getBinaryOperatorPrecedence(lexing::TokenType tokenType)
    {
        switch (tokenType)
        {
            case lexing::TokenType::DoubleColon:
                return 100;

            case lexing::TokenType::LeftParen:
            case lexing::TokenType::LeftSquareBracket:
            case lexing::TokenType::Dot:
            case lexing::TokenType::RightArrow:
                return 90;

            case lexing::TokenType::Star:
            case lexing::TokenType::Slash:
                return 75;

            case lexing::TokenType::Plus:
            case lexing::TokenType::Minus:
                return 70;

            case lexing::TokenType::LessThan:
            case lexing::TokenType::GreaterThan:
            case lexing::TokenType::LessEqual:
            case lexing::TokenType::GreaterEqual:
                return 55;

            case lexing::TokenType::DoubleEquals:
            case lexing::TokenType::BangEquals:
                return 50;

            case lexing::TokenType::Ampersand:
                return 45;
            case lexing::TokenType::Caret:
                return 40;
            case lexing::TokenType::Pipe:
                return 35;

            case lexing::TokenType::Equals:
            case lexing::TokenType::PlusEquals:
            case lexing::TokenType::MinusEquals:
                return 20;

            default:
                return 0;
        }
    }

    int Parser::getPrefixUnaryOperatorPrecedence(lexing::TokenType tokenType)
    {
        switch(tokenType)
        {
            case lexing::TokenType::Minus:
            case lexing::TokenType::Tilde:
            case lexing::TokenType::Ampersand:
            case lexing::TokenType::Star:
            case lexing::TokenType::LeftParen:
                return 85;

            default:
                return 0;
        }
    }

    int Parser::getPostfixUnaryOperatorPrecedence(lexing::TokenType tokenType)
    {
        switch(tokenType)
        {
            default:
                return 0;
        }
    }

    Type* Parser::parseType(bool failable)
    {
        int startPosition = mPosition;
        Type* type = nullptr;
        if (current().getTokenType() == lexing::TokenType::StructKeyword)
        {
            consume();
            std::vector<std::string> names;
            expectToken(lexing::TokenType::Identifier);
            while (current().getTokenType() == lexing::TokenType::Identifier)
            {
                names.push_back(consume().getText());
                if (current().getTokenType() == lexing::TokenType::DoubleColon)
                {
                    consume();
                }
            }
            std::vector<std::string> types = symbol::GetSymbol(names, mNamespaces);
            for (auto& name : types)
            {
                type = StructType::Get(name);
                if (type) break;
            }
            if (!type)
            {
                if (failable)
                {
                    mPosition = startPosition;
                    return nullptr;
                }
                mDiag.compilerError(peek(-1).getStart(), peek(-1).getEnd(), std::format("unknown type name '{}{}{}'", fmt::bold, names.back(), fmt::defaults));
            }
        }
        else
        {
            std::vector<std::string> names;
            if (current().getTokenType() == lexing::TokenType::Type)
            {
                names.push_back(consume().getText());
            }
            else
            {
                while (current().getTokenType() == lexing::TokenType::Identifier)
                {
                    names.push_back(consume().getText());
                    if (current().getTokenType() == lexing::TokenType::DoubleColon)
                    {
                        consume();
                    }
                }
            }

            std::vector<std::string> types = symbol::GetSymbol(names, mNamespaces);

            lexing::Token token = peek(-1);
            for (auto& name : types)
            {
                type = Type::Get(name);
                if (type) break;
            }
            if (!type)
            {
                if (!names.empty())
                    type = Type::Get(names.front());

                if (!type)
                {
                    if (failable)
                    {
                        mPosition = startPosition;
                        return nullptr;
                    }

                    mDiag.compilerError(token.getStart(), token.getEnd(), std::format("unknown type name '{}{}{}'", fmt::bold, token.getText(), fmt::defaults));
                }
            }
        }

        while(current().getTokenType() == lexing::TokenType::Star || current().getTokenType() == lexing::TokenType::LeftSquareBracket)
        {
            if (current().getTokenType() == lexing::TokenType::Star)
            {
                consume();
                type = PointerType::Create(type);
            }
            else
            {
                consume();
                expectToken(lexing::TokenType::IntegerLiteral);
                int count = std::stoi(consume().getText(), 0, 0);
                expectToken(lexing::TokenType::RightSquareBracket);
                consume();
                type = ArrayType::Create(type, count);
            }
        }

        return type;
    }

    ASTNodePtr Parser::parseExpression(Type* preferredType, int precedence)
    {
        ASTNodePtr lhs;
        int prefixOperatorPrecedence = getPrefixUnaryOperatorPrecedence(current().getTokenType());
        if (prefixOperatorPrecedence >= precedence)
        {
            lexing::Token operatorToken = consume();
            if (operatorToken.getTokenType() == lexing::TokenType::LeftParen) // Cast expression or parenthesized expression
            {
                if (Type* type = parseType(true)) // Cast
                {
                    expectToken(lexing::TokenType::RightParen);
                    consume();
                    lhs = std::make_unique<CastExpression>(parseExpression(nullptr, prefixOperatorPrecedence), type, std::move(operatorToken));
                }
                else // Parenthesized expression
                {
                    lhs = parseParenthesizedExpression(preferredType);
                }
            }
            else
            {
                lhs = std::make_unique<UnaryExpression>(parseExpression(preferredType, prefixOperatorPrecedence), operatorToken.getTokenType());
            }
        }
        else
        {
            lhs = parsePrimary(preferredType);
        }

        while(true)
        {
            int binaryOperatorPrecedence = getBinaryOperatorPrecedence(current().getTokenType());
            if (binaryOperatorPrecedence < precedence)
            {
                break;
            }

            lexing::Token operatorToken = consume();

            if (operatorToken.getTokenType() == lexing::TokenType::LeftParen)
            {
                lhs = parseCallExpression(std::move(lhs));
            }
            else if (operatorToken.getTokenType() == lexing::TokenType::Dot)
            {
                lhs = parseMemberAccess(std::move(lhs), false);
            }
            else if (operatorToken.getTokenType() == lexing::TokenType::RightArrow)
            {
                lhs = parseMemberAccess(std::move(lhs), true);
            }
            else if (operatorToken.getTokenType() == lexing::TokenType::DoubleColon)
            {
                lexing::Token token = current();
                lhs = std::make_unique<ScopeResolution>(std::move(lhs), token, parseExpression(nullptr, binaryOperatorPrecedence));
            }
            else
            {
                ASTNodePtr rhs = parseExpression(lhs->getType(), binaryOperatorPrecedence);
                lhs = std::make_unique<BinaryExpression>(std::move(lhs), std::move(operatorToken), std::move(rhs));
            }

            if (operatorToken.getTokenType() == lexing::TokenType::LeftSquareBracket)
            {
                expectToken(lexing::TokenType::RightSquareBracket);
                consume();
            }
        }

        return lhs;
    }

    ASTNodePtr Parser::parsePrimary(Type* preferredType)
    {
        switch (current().getTokenType())
        {
            case lexing::TokenType::LeftBracket:
                return parseCompoundStatement();

            case lexing::TokenType::ReturnKeyword:
                return parseReturnStatement();

            case lexing::TokenType::LetKeyword:
                return parseVariableDeclaration();

            case lexing::TokenType::IfKeyword:
                return parseIfStatement();
            case lexing::TokenType::WhileKeyword:
                return parseWhileStatement();
            case lexing::TokenType::ForKeyword:
                return parseForStatement();
            case lexing::TokenType::BreakKeyword:
                return std::make_unique<BreakStatement>(std::move(consume()));
            case lexing::TokenType::ContinueKeyword:
                return std::make_unique<ContinueStatement>(std::move(consume()));

            case lexing::TokenType::TrueKeyword:
                consume();
                return std::make_unique<BooleanLiteral>(true);
            case lexing::TokenType::FalseKeyword:
                consume();
                return std::make_unique<BooleanLiteral>(false);

            case lexing::TokenType::NullptrKeyword:
                consume();
                return std::make_unique<NullptrLiteral>(preferredType);

            case lexing::TokenType::SizeofKeyword:
                return parseSizeof(preferredType);

            case lexing::TokenType::IntegerLiteral:
                return parseIntegerLiteral(preferredType);
            case lexing::TokenType::StringLiteral:
                return parseStringLiteral();

            case lexing::TokenType::Identifier:
                return parseVariableExpression(preferredType);

            case lexing::TokenType::StructKeyword:
                return parseStructInitializer();

            case lexing::TokenType::LeftSquareBracket:
                return parseArrayInitializer(preferredType);

            default:
                mDiag.compilerError(current().getStart(), current().getEnd(), "Unexpected token. Expected primary expression");
        }
    }

    ASTNodePtr Parser::parseParenthesizedExpression(Type* preferredType)
    {
        ASTNodePtr expression = parseExpression(preferredType);

        expectToken(lexing::TokenType::RightParen);
        consume();

        return expression;
    }

    FunctionPtr Parser::parseFunction(std::vector<GlobalAttribute> attributes)
    {
        consume();

        std::optional<std::string> struc;
        std::optional<lexing::Token> structNameToken;

        if (current().getTokenType() == lexing::TokenType::Identifier)
        {
            structNameToken = current();
            struc = consume().getText();
        }

        expectToken(lexing::TokenType::Asperand);
        consume();

        expectToken(lexing::TokenType::Identifier);
        std::string name = consume().getText();

        expectToken(lexing::TokenType::LeftParen);
        consume();

        std::vector<FunctionArgument> arguments;
        StructType* structType = nullptr;

        Scope* functionScope = new Scope(mScope, structType);
        mScope = functionScope;

        if (struc.has_value())
        {
            std::vector<std::string> names = mNamespaces;
            names.push_back(struc.value());
            std::vector<std::string> types = symbol::GetSymbol(names, {});
            for (auto name : types)
            {
                structType = StructType::Get(name);
                if (structType) break;
            }

            if (!structType)
            {
                mDiag.compilerError(structNameToken->getStart(),structNameToken->getEnd(), std::format("unknown type name {}", *struc));
            }
            functionScope->owner = structType;

            mScope->locals["this"] = LocalSymbol(nullptr, PointerType::Create(structType));
            arguments.push_back({"this", PointerType::Create(structType)});
        }

        while (current().getTokenType() != lexing::TokenType::RightParen)
        {
            expectToken(lexing::TokenType::Identifier);
            std::string name = consume().getText();

            expectToken(lexing::TokenType::Colon);
            consume();

            Type* type = parseType();
            mScope->locals[name] = LocalSymbol(nullptr, type);
            arguments.push_back({std::move(name), type});

            if (current().getTokenType() != lexing::TokenType::RightParen)
            {
                expectToken(lexing::TokenType::Comma);
                consume();
            }
        }
        consume();

        Type* type = Type::Get("void");
        if (current().getTokenType() == lexing::TokenType::RightArrow)
        {
            consume();
            type = parseType();
        }

        mSymbols.push_back({name, type});

        if (current().getTokenType() == lexing::TokenType::Semicolon) // Extern function declaration
        {
            consume();
            return std::make_unique<Function>(std::move(attributes), type, std::move(arguments), std::move(struc), std::move(name), std::vector<ASTNodePtr>(), nullptr);
        }

        expectEitherToken({lexing::TokenType::LeftBracket, lexing::TokenType::Equals});
        bool isExpressionBodied = current().getTokenType() == lexing::TokenType::Equals;
        consume();

        std::vector<ASTNodePtr> body;
        if (isExpressionBodied)
        {
            ASTNodePtr exp = parseExpression();
            if (!type->isVoidType())
                body.push_back(std::make_unique<ReturnStatement>(std::move(exp)));
            else
                body.push_back(std::move(exp));
            expectToken(lexing::TokenType::Semicolon);
            consume();
        }
        else
        {
            while (current().getTokenType() != lexing::TokenType::RightBracket)
            {
                body.push_back(parseExpression());
                expectToken(lexing::TokenType::Semicolon);
                consume();
            }
            consume();
        }

        mScope = functionScope->parent;

        return std::make_unique<Function>(std::move(attributes), type, std::move(arguments), std::move(struc), std::move(name), std::move(body), functionScope);
    }

    NamespacePtr Parser::parseNamespace()
    {
        consume(); // namespace

        expectToken(lexing::TokenType::Identifier);
        std::string name = consume().getText();
        mNamespaces.push_back(name);

        expectToken(lexing::TokenType::LeftBracket);
        consume();

        Scope* scope = new Scope(mScope, nullptr);
        mScope = scope;
        
        std::vector<ASTNodePtr> body;
        while(current().getTokenType() != lexing::TokenType::RightBracket)
        {
            ASTNodePtr node = parseGlobal(body);
            if (node)
            {
                body.push_back(std::move(node));
            }
        }
        consume();
        mSymbols.push_back({name, nullptr});

        mScope = scope->parent;
        mNamespaces.pop_back();

        return std::make_unique<Namespace>(std::move(name), std::move(body), scope);
    }

    StructDeclarationPtr Parser::parseStructDeclaration()
    {
        consume(); // struct

        expectToken(lexing::TokenType::Identifier);
        std::string name = consume().getText();
        std::vector<std::string> names = mNamespaces;
        names.push_back(name);

        expectToken(lexing::TokenType::LeftBracket);
        consume();

        StructType* structType = StructType::Create(names, {});

        std::vector<StructType::Field>& fieldTypes = structType->getFields();
        std::vector<StructField> fields;
        std::vector<StructMethod> methods;
        while (current().getTokenType() != lexing::TokenType::RightBracket)
        {
            bool priv = false;
            if (current().getTokenType() == lexing::TokenType::PrivateKeyword)
            {
                consume();
                priv = true;
            }

            if (current().getTokenType() == lexing::TokenType::FuncKeyword)
            {
                consume();
                expectToken(lexing::TokenType::Asperand);
                consume();

                expectToken(lexing::TokenType::Identifier);
                std::string name = consume().getText();

                expectToken(lexing::TokenType::LeftParen);
                consume();

                std::vector<FunctionArgument> arguments;
                while (current().getTokenType() != lexing::TokenType::RightParen)
                {
                    expectToken(lexing::TokenType::Identifier);
                    std::string name = consume().getText();

                    expectToken(lexing::TokenType::Colon);
                    consume();

                    Type* type = parseType();
                    arguments.push_back({std::move(name), type});

                    if (current().getTokenType() != lexing::TokenType::RightParen)
                    {
                        expectToken(lexing::TokenType::Comma);
                        consume();
                    }
                }
                consume();

                Type* type = Type::Get("void");
                if (current().getTokenType() == lexing::TokenType::RightArrow)
                {
                    consume();
                    type = parseType();
                }

                expectToken(lexing::TokenType::Semicolon);

                if (current().getTokenType() == lexing::TokenType::Semicolon)
                {
                    consume();
                    methods.push_back({priv, std::move(name), type, std::move(arguments), std::vector<ASTNodePtr>(), nullptr});
                    continue;
                }

                // if definition in struct will ever be readded

                Scope* scope = new Scope(mScope, /*structType*/nullptr);
                mScope = scope;

                //mSymbols.push_back({"this", PointerType::Create(structType)});

                expectToken(lexing::TokenType::LeftBracket);
                consume();

                std::vector<ASTNodePtr> body;
                while (current().getTokenType() != lexing::TokenType::RightBracket)
                {
                    body.push_back(parseExpression());
                    expectToken(lexing::TokenType::Semicolon);
                    consume();
                }
                consume();

                mScope = mScope->parent;

                methods.push_back({priv, name, type, std::move(arguments), std::move(body), ScopePtr(scope)});
            }
            else
            {
                expectToken(lexing::TokenType::Identifier);
                std::string name = consume().getText();

                expectToken(lexing::TokenType::Colon);
                consume();

                Type* type = parseType();

                fieldTypes.push_back({priv, name, type});
                fields.push_back({priv, std::move(name), type});

                expectToken(lexing::TokenType::Semicolon);
                consume();
            }
        }
        consume();

        return std::make_unique<StructDeclaration>(std::move(names), std::move(fields), std::move(methods), structType);
    }

    GlobalDeclarationPtr Parser::parseGlobalDeclaration()
    {
        consume(); // global

        expectToken(lexing::TokenType::Identifier);
        std::vector<std::string> names = mNamespaces;
        names.push_back(consume().getText());

        expectToken(lexing::TokenType::Colon);
        consume();

        Type* type = parseType();

        expectToken(lexing::TokenType::Equals);
        consume();

        ASTNodePtr initVal = parseExpression(type);

        expectToken(lexing::TokenType::Semicolon);
        consume();

        mSymbols.push_back({names.back(), type});

        return std::make_unique<GlobalDeclaration>(std::move(names), type, std::move(initVal));
    }

    std::pair<std::vector<ASTNodePtr>, std::vector<GlobalSymbol>> Parser::parseImportStatement()
    {
        consume(); // import

        std::filesystem::path path;
        while (current().getTokenType() != lexing::TokenType::Semicolon)
        {
            expectToken(lexing::TokenType::Identifier);
            path /= consume().getText();

            if (current().getTokenType() != lexing::TokenType::Semicolon)
            {
                expectToken(lexing::TokenType::Dot);
                consume();
            }
        }
        consume();

        return mImportManager.ImportSymbols(path, mDiag);
    }
    
    UsingDeclarationPtr Parser::parseUsingDeclaration()
    {
        consume(); // using

        std::vector<std::string> names = mNamespaces;
        names.push_back(consume().getText());

        expectToken(lexing::TokenType::Equals);
        consume();

        Type* type = parseType();

        expectToken(lexing::TokenType::Semicolon);
        consume();

        return std::make_unique<UsingDeclaration>(std::move(names), type);
    }

    EnumDeclarationPtr Parser::parseEnumDeclaration()
    {
        consume(); // enum

        std::vector<std::string> names = mNamespaces;
        names.push_back(consume().getText());

        expectToken(lexing::TokenType::LeftBracket);
        consume();

        std::vector<EnumField> fields;
        int currentValue = 0;
        while (current().getTokenType() != lexing::TokenType::RightBracket)
        {
            expectToken(lexing::TokenType::Identifier);
            std::string name = consume().getText();

            if (current().getTokenType() == lexing::TokenType::Equals)
            {
                consume();
                IntegerLiteralPtr literal = parseIntegerLiteral();
                currentValue = literal->getValue();
            }

            fields.push_back({std::move(name), currentValue++});

            if (current().getTokenType() != lexing::TokenType::RightBracket)
            {
                expectToken(lexing::TokenType::Comma);
                consume();
            }
        }
        consume();

        return std::make_unique<EnumDeclaration>(std::move(names), std::move(fields));
    }

    CompoundStatementPtr Parser::parseCompoundStatement()
    {
        consume(); // left bracket

        Scope* blockScope = new Scope(mScope, nullptr);
        mScope = blockScope;

        std::vector<ASTNodePtr> body;
        while (current().getTokenType() != lexing::TokenType::RightBracket)
        {
            body.push_back(parseExpression());
            expectToken(lexing::TokenType::Semicolon);
            consume();
        }
        consume();

        mTokens.insert(mTokens.begin()+mPosition, lexing::Token(lexing::TokenType::Semicolon, {0, 0}, {0, 0}));

        mScope = blockScope->parent;

        return std::make_unique<CompoundStatement>(std::move(body), blockScope);
    }

    ReturnStatementPtr Parser::parseReturnStatement()
    {
        consume();

        if (current().getTokenType() == lexing::TokenType::Semicolon)
        {
            return std::make_unique<ReturnStatement>(nullptr);
        }

        return std::make_unique<ReturnStatement>(parseExpression()); // TODO: Pass preferred type as current function return type
    }

    VariableDeclarationPtr Parser::parseVariableDeclaration()
    {
        consume(); // let

        std::string name = consume().getText();

        expectToken(lexing::TokenType::Colon);
        consume();

        Type* type = parseType();
        
        mScope->locals[name] = LocalSymbol(nullptr, type);

        if (current().getTokenType() == lexing::TokenType::Semicolon)
        {
            return std::make_unique<VariableDeclaration>(type, std::move(name), nullptr);
        }

        expectToken(lexing::TokenType::Equals);
        consume();

        return std::make_unique<VariableDeclaration>(type, std::move(name), parseExpression(type));
    }

    IfStatementPtr Parser::parseIfStatement()
    {
        consume(); // if

        expectToken(lexing::TokenType::LeftParen);
        consume();

        ASTNodePtr condition = parseExpression();

        expectToken(lexing::TokenType::RightParen);
        consume();

        ASTNodePtr body = parseExpression();
        ASTNodePtr elseBody = nullptr;

        if (peek(1).getTokenType() == lexing::TokenType::ElseKeyword)
        {
            expectToken(lexing::TokenType::Semicolon);
            consume();

            consume(); // else

            elseBody = parseExpression();
        }

        return std::make_unique<IfStatement>(std::move(condition), std::move(body), std::move(elseBody));
    }

    WhileStatementPtr Parser::parseWhileStatement()
    {
        consume(); // while

        expectToken(lexing::TokenType::LeftParen);
        consume();

        Scope* whileScope = new Scope(mScope, nullptr);
        mScope = whileScope;

        ASTNodePtr condition = parseExpression();

        expectToken(lexing::TokenType::RightParen);
        consume();

        ASTNodePtr body = parseExpression();

        mScope = whileScope->parent;

        return std::make_unique<WhileStatement>(std::move(condition), std::move(body), whileScope);
    }

    ForStatementPtr Parser::parseForStatement()
    {
        consume();

        expectToken(lexing::TokenType::LeftParen);
        consume();

        ASTNodePtr init = nullptr;
        ASTNodePtr condition = nullptr;
        std::vector<ASTNodePtr> loopExpr;

        Scope* forScope = new Scope(mScope, nullptr);
        mScope = forScope;

        if (current().getTokenType() != lexing::TokenType::Semicolon)
        {
            init = parseExpression();
            expectToken(lexing::TokenType::Semicolon);
        }
        consume();

        if (current().getTokenType() != lexing::TokenType::Semicolon)
        {
            condition = parseExpression();
            expectToken(lexing::TokenType::Semicolon);
        }
        consume();

        if (current().getTokenType() != lexing::TokenType::RightParen)
        {
            while (current().getTokenType() != lexing::TokenType::RightParen)
            {
                loopExpr.push_back(parseExpression());

                if (current().getTokenType() != lexing::TokenType::RightParen)
                {
                    expectToken(lexing::TokenType::Comma);
                    consume();
                }
            }
        }
        consume();

        ASTNodePtr body = parseExpression();

        mScope = forScope->parent;

        return std::make_unique<ForStatement>(std::move(init), std::move(condition), std::move(loopExpr), std::move(body), forScope);
    }

    SizeofExpressionPtr Parser::parseSizeof(Type* preferredType)
    {
        consume();
        expectToken(lexing::TokenType::LeftParen);
        consume();

        Type* type = parseType();

        expectToken(lexing::TokenType::RightParen);
        consume();

        return std::make_unique<SizeofExpression>(preferredType, type);
    }

    IntegerLiteralPtr Parser::parseIntegerLiteral(Type* preferredType)
    {
        return std::make_unique<IntegerLiteral>(std::stoull(consume().getText(), 0, 0), preferredType);
    }

    StringLiteralPtr Parser::parseStringLiteral()
    {
        return std::make_unique<StringLiteral>(consume().getText());
    }

    VariableExpressionPtr Parser::parseVariableExpression(Type*)
    {
        lexing::Token nameToken = current();
        std::string name = consume().getText();

        auto local = mScope->findVariable(name);
        if (local)
        {
            return std::make_unique<VariableExpression>(std::move(name), local->type, std::move(nameToken));
        }

        auto it = std::find_if(mSymbols.begin(), mSymbols.end(), [&name](const GlobalSymbol& symbol) {
            return symbol.name == name;
        });

        if (it != mSymbols.end())
        {
            return std::make_unique<VariableExpression>(std::move(name), it->type, std::move(nameToken));
        }

        mDiag.compilerError(nameToken.getStart(), nameToken.getEnd(), std::format("Unknown symbol '{}{}{}'", fmt::bold, name, fmt::defaults));
    }

    CallExpressionPtr Parser::parseCallExpression(ASTNodePtr function)
    {
        std::vector<ASTNodePtr> parameters;
        while (current().getTokenType() != lexing::TokenType::RightParen)
        {
            parameters.push_back(parseExpression());

            if (current().getTokenType() != lexing::TokenType::RightParen)
            {
                expectToken(lexing::TokenType::Comma);
                consume();
            }
        }
        consume();

        return std::make_unique<CallExpression>(std::move(function), std::move(parameters));
    }

    MemberAccessPtr Parser::parseMemberAccess(ASTNodePtr struc, bool pointer)
    {
        lexing::Token nameToken = consume();

        return std::make_unique<MemberAccess>(std::move(struc), std::string(nameToken.getText()), pointer, std::move(nameToken));
    }

    StructInitializerPtr Parser::parseStructInitializer()
    {
        StructType* type = static_cast<StructType*>(parseType());

        expectToken(lexing::TokenType::LeftBracket);
        consume();

        std::vector<ASTNodePtr> body;
        int index = 0;
        while (current().getTokenType() != lexing::TokenType::RightBracket)
        {
            body.push_back(parseExpression(type->getFields()[index++].type));

            if (current().getTokenType() != lexing::TokenType::RightBracket)
            {
                expectToken(lexing::TokenType::Comma);
                consume();
            }
        }
        consume();

        return std::make_unique<StructInitializer>(type, std::move(body));
    }

    ArrayInitializerPtr Parser::parseArrayInitializer(Type* preferredType)
    {
        consume(); // left square bracket

        preferredType = static_cast<ArrayType*>(preferredType)->getBaseType();

        std::vector<ASTNodePtr> values;
        while (current().getTokenType() != lexing::TokenType::RightSquareBracket)
        {
            values.push_back(parseExpression(preferredType));

            if (current().getTokenType() != lexing::TokenType::RightSquareBracket)
            {
                expectToken(lexing::TokenType::Comma);
                consume();
            }
        }
        consume();

        return std::make_unique<ArrayInitializer>(std::move(values));
    }

    void Parser::parseAttributes(std::vector<GlobalAttribute>& attributes)
    {
        consume(); // [[

        while (current().getTokenType() != lexing::TokenType::DoubleRightSquareBracket)
        {
            lexing::Token token = consume();

            if (token.getText() == "NoMangle")
            {
                attributes.push_back(GlobalAttribute(GlobalAttributeType::NoMangle));
            }
            else
            {
                mDiag.compilerError(token.getStart(), token.getEnd(), std::format("unknown attribute '{}{}{}'", fmt::bold, token.getText(), fmt::defaults));
            }

            if (current().getTokenType() != lexing::TokenType::DoubleRightSquareBracket)
            {
                expectToken(lexing::TokenType::Comma);
                consume();
            }
        }
        consume();
    }
}
