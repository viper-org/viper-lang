// Copyright 2023 solar-mist


#include "parser/Parser.h"

#include "parser/ast/expression/BinaryExpression.h"
#include "parser/ast/expression/UnaryExpression.h"
#include "parser/ast/expression/BooleanLiteral.h"
#include "parser/ast/expression/NullptrLiteral.h"
#include "parser/ast/expression/CastExpression.h"

#include "lexer/Token.h"

#include "type/PointerType.h"
#include "type/StructType.h"
#include "type/ArrayType.h"

#include <algorithm>
#include <format>
#include <iostream>

namespace parser
{
    Parser::Parser(std::vector<lexing::Token>& tokens)
        : mTokens(tokens)
        , mPosition(0)
        , mScope(nullptr)
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
            lexing::Token temp(tokenType);
            std::cerr << "Expected " << temp.toString() << ". Found " << current().toString() << "\n";
            std::exit(1);
        }
    }

    std::vector<ASTNodePtr> Parser::parse()
    {
        std::vector<ASTNodePtr> result;

        while (mPosition < mTokens.size())
        {
            result.push_back(parseGlobal());
        }

        return result;
    }

    ASTNodePtr Parser::parseGlobal()
    {
        switch (current().getTokenType())
        {
            case lexing::TokenType::FuncKeyword:
                return parseFunction();
            case lexing::TokenType::StructKeyword:
                return parseStructDeclaration();
            case lexing::TokenType::GlobalKeyword:
                return parseGlobalDeclaration();
            default:
                std::cerr << "Unexpected token: " << current().toString() << ". Expected global statement.\n";
                std::exit(1);
        }
    }

    int Parser::getBinaryOperatorPrecedence(lexing::TokenType tokenType)
    {
        switch (tokenType)
        {
            case lexing::TokenType::LeftParen:
            case lexing::TokenType::LeftSquareBracket:
            case lexing::TokenType::Dot:
            case lexing::TokenType::RightArrow:
                return 90;

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
    
    Type* Parser::parseType()
    {
        Type* type;
        if (current().getTokenType() == lexing::TokenType::StructKeyword)
        {
            consume();
            expectToken(lexing::TokenType::Identifier);
            type = StructType::Get(consume().getText());
        }
        else
        {
            expectToken(lexing::TokenType::Type);
            type = Type::Get(consume().getText());
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
            lexing::TokenType operatorTokenType = consume().getTokenType();
            if (operatorTokenType == lexing::TokenType::LeftParen) // Cast expression
            {
                Type* type = parseType();
                expectToken(lexing::TokenType::RightParen);
                consume();
                lhs = std::make_unique<CastExpression>(parseExpression(nullptr, prefixOperatorPrecedence), type);
            }
            else
            {
                lhs = std::make_unique<UnaryExpression>(parseExpression(nullptr, prefixOperatorPrecedence), operatorTokenType);
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

            lexing::TokenType operatorTokenType = consume().getTokenType();

            if (operatorTokenType == lexing::TokenType::LeftParen)
            {
                lhs = parseCallExpression(std::move(lhs));
            }
            else if (operatorTokenType == lexing::TokenType::Dot)
            {
                lhs = parseMemberAccess(std::move(lhs), false);
            }
            else if (operatorTokenType == lexing::TokenType::RightArrow)
            {
                lhs = parseMemberAccess(std::move(lhs), true);
            }
            else
            {
                ASTNodePtr rhs = parseExpression(preferredType, precedence);
                lhs = std::make_unique<BinaryExpression>(std::move(lhs), operatorTokenType, std::move(rhs));
            }

            if (operatorTokenType == lexing::TokenType::LeftSquareBracket)
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

            case lexing::TokenType::TrueKeyword:
                consume();
                return std::make_unique<BooleanLiteral>(true);
            case lexing::TokenType::FalseKeyword:
                consume();
                return std::make_unique<BooleanLiteral>(false);

            case lexing::TokenType::NullptrKeyword:
                consume();
                return std::make_unique<NullptrLiteral>(preferredType);

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
                std::cerr << "Unexpected token. Expected primary expression.\n";
                std::exit(1);
        }
    }

    FunctionPtr Parser::parseFunction()
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
            mSymbols.push_back({name, type});
            arguments.push_back({std::move(name), type});

            if (current().getTokenType() != lexing::TokenType::RightParen)
            {
                expectToken(lexing::TokenType::Comma);
                consume();
            }
        }
        consume();

        expectToken(lexing::TokenType::RightArrow);
        consume();
        
        Type* type = parseType();

        mSymbols.push_back({name, type});

        if (current().getTokenType() == lexing::TokenType::Semicolon) // Extern function declaration
        {
            consume();
            return std::make_unique<Function>(type, std::move(arguments), std::move(name), std::vector<ASTNodePtr>(), nullptr);
        }

        Scope* functionScope = new Scope(mScope);
        mScope = functionScope;

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

        mScope = functionScope->parent;

        return std::make_unique<Function>(type, std::move(arguments), std::move(name), std::move(body), functionScope);
    }

    StructDeclarationPtr Parser::parseStructDeclaration()
    {
        consume(); // struct

        expectToken(lexing::TokenType::Identifier);
        std::string name = consume().getText();

        expectToken(lexing::TokenType::LeftBracket);
        consume();

        std::vector<StructField> fields;
        while (current().getTokenType() != lexing::TokenType::RightBracket)
        {
            expectToken(lexing::TokenType::Identifier);
            std::string name = consume().getText();

            expectToken(lexing::TokenType::Colon);
            consume();

            Type* type = parseType();

            fields.push_back({std::move(name), type});

            expectToken(lexing::TokenType::Semicolon);
            consume();
        }
        consume();

        return std::make_unique<StructDeclaration>(std::move(name), std::move(fields));
    }

    GlobalDeclarationPtr Parser::parseGlobalDeclaration()
    {
        consume(); // global

        expectToken(lexing::TokenType::Identifier);
        std::string name = consume().getText();

        expectToken(lexing::TokenType::Colon);
        consume();

        Type* type = parseType();

        expectToken(lexing::TokenType::Equals);
        consume();

        ASTNodePtr initVal = parseExpression(type);

        expectToken(lexing::TokenType::Semicolon);
        consume();

        mSymbols.push_back({name, type});

        return std::make_unique<GlobalDeclaration>(std::move(name), type, std::move(initVal));
    }

    CompoundStatementPtr Parser::parseCompoundStatement()
    {
        consume(); // left bracket

        Scope* blockScope = new Scope(mScope);
        mScope = blockScope;
        
        std::vector<ASTNodePtr> body;
        while (current().getTokenType() != lexing::TokenType::RightBracket)
        {
            body.push_back(parseExpression());
            expectToken(lexing::TokenType::Semicolon);
            consume();
        }
        consume();

        mTokens.insert(mTokens.begin()+mPosition, lexing::Token(lexing::TokenType::Semicolon));

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

        mSymbols.push_back({name, type});

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

        ASTNodePtr condition = parseExpression();

        expectToken(lexing::TokenType::RightParen);
        consume();

        ASTNodePtr body = parseExpression();

        return std::make_unique<WhileStatement>(std::move(condition), std::move(body));
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
        std::string name = consume().getText();

        auto it = std::find_if(mSymbols.begin(), mSymbols.end(), [&name](const Symbol& symbol) {
            return symbol.name == name;
        });

        if (it != mSymbols.end())
        {
            return std::make_unique<VariableExpression>(std::move(name), it->type);
        }

        std::cerr << std::format("Unknown local symbol '{}'", name);
        std::exit(1);
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
        return std::make_unique<MemberAccess>(std::move(struc), consume().getText(), pointer);
    }

    StructInitializerPtr Parser::parseStructInitializer()
    {
        consume(); // struct

        StructType* type = StructType::Get(consume().getText());
        
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
}