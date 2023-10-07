// Copyright 2023 solar-mist


#include "parser/Parser.h"

#include "parser/ast/expression/UnaryExpression.h"
#include "parser/ast/expression/BinaryExpression.h"
#include "parser/ast/expression/AsExpression.h"

#include "lexer/Token.h"

#include <iostream>

namespace parsing
{
    Parser::Parser(std::vector<lexing::Token>& tokens, Environment* scope)
        : mTokens(tokens)
        , mPosition(0)
        , mScope(scope)
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

    int Parser::getBinaryOperatorPrecedence(lexing::TokenType tokenType)
    {
        switch (tokenType)
        {
            
            case lexing::TokenType::LeftParen:
                return 55;

            case lexing::TokenType::AsKeyword:
                return 50;

            case lexing::TokenType::Plus:
            case lexing::TokenType::Minus:
                return 35;

            case lexing::TokenType::DoubleEquals:
                return 25;

            case lexing::TokenType::Equals:
                return 10;
            default:
                return 0;
        }
    }

    int Parser::getUnaryOperatorPrecedence(lexing::TokenType tokenType)
    {
        switch (tokenType)
        {
            case lexing::TokenType::Asperand:
            case lexing::TokenType::Star:
                return 50;
            default:
                return 0;
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
            case lexing::TokenType::Type:
                return parseFunction();
            case lexing::TokenType::ExternKeyword:
                return parseExternFunction();
            default:
                std::cerr << "Unexpected token: " << current().toString() << ". Expected global statement.\n";
                std::exit(1);
        }
    }

    ASTNodePtr Parser::parseExpression(int precedence)
    {
        ASTNodePtr lhs;
        int unaryOperatorPrecedence = getUnaryOperatorPrecedence(current().getTokenType());
        if (unaryOperatorPrecedence && unaryOperatorPrecedence >= precedence)
        {
            lexing::TokenType operatorToken = consume().getTokenType();
            lhs = std::make_unique<UnaryExpression>(parseExpression(unaryOperatorPrecedence), operatorToken);
        }
        else
        {
            lhs = parsePrimary();
        }

        while (true)
        {
            int binaryOperatorPrecedence = getBinaryOperatorPrecedence(current().getTokenType());
            if (binaryOperatorPrecedence < precedence)
            {
                break;
            }
            
            lexing::Token operatorToken = consume();
            if (operatorToken == lexing::TokenType::LeftParen)
            {
                lhs = parseCallExpression(std::move(lhs));
            }
            else if (operatorToken == lexing::TokenType::AsKeyword)
            {
                Type* destination = parseType();
                lhs = std::make_unique<AsExpression>(std::move(lhs), destination);
            }
            else
            {
                ASTNodePtr rhs = parseExpression(binaryOperatorPrecedence);
                lhs = std::make_unique<BinaryExpression>(std::move(lhs), operatorToken.getTokenType(), std::move(rhs));
            }
        }

        return lhs;
    }

    ASTNodePtr Parser::parsePrimary()
    {
        switch (current().getTokenType())
        {
            case lexing::TokenType::ReturnKeyword:
                return parseReturnStatement();

            case lexing::TokenType::IntegerLiteral:
                return parseIntegerLiteral();

            case lexing::TokenType::Type:
                return parseVariableDeclaration();

            case lexing::TokenType::Identifier:
                return parseVariable();

            case lexing::TokenType::IfKeyword:
                return parseIfStatement();

            case lexing::TokenType::LeftBracket:
                return parseCompoundStatement();
            default:
                std::cerr << "Unexpected token. Expected primary expression.\n";
                std::exit(1);
        }
    }

    Type* Parser::parseType()
    {
        expectToken(lexing::TokenType::Type);

        Type* type = Type::Get(consume().getText());
        
        while (current().getTokenType() == lexing::TokenType::Star)
        {
            consume();
            type = Type::GetPointerType(type);
        }

        return type;
    }

    FunctionPtr Parser::parseFunction()
    {
        Type* type = parseType();

        expectToken(lexing::TokenType::Identifier);
        std::string name = consume().getText();

        std::vector<FunctionArgument> arguments;
        expectToken(lexing::TokenType::LeftParen);
        consume();
        while (current().getTokenType() != lexing::TokenType::RightParen)
        {
            Type* type = parseType();
            std::string name = consume().getText();
            arguments.emplace_back(name, type);
            if (current().getTokenType() != lexing::TokenType::RightParen)
            {
                expectToken(lexing::TokenType::Comma);
                consume();
            }
        }
        consume();

        Environment* outerScope = mScope;
        Environment* scope = new Environment;
        scope->parent = outerScope;
        mScope = scope;

        for (const auto& argument : arguments)
        {
            mScope->symbols[argument.getName()] = Symbol(false, argument.getType(), argument.getName());
        }

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

        mGlobalSymbols[name] = Symbol(true, type, name);

        mScope = outerScope;

        return std::make_unique<Function>(type, name, std::move(arguments), std::move(body), scope);
    }

    ExternFunctionPtr Parser::parseExternFunction()
    {
        consume(); // ExternKeyword
        
        Type* type = parseType();

        expectToken(lexing::TokenType::Identifier);
        std::string name = consume().getText();

        std::vector<FunctionArgument> arguments;
        expectToken(lexing::TokenType::LeftParen);
        consume();
        while (current().getTokenType() != lexing::TokenType::RightParen)
        {
            Type* type = parseType();
            std::string name = consume().getText();
            arguments.emplace_back(name, type);
            if (current().getTokenType() != lexing::TokenType::RightParen)
            {
                expectToken(lexing::TokenType::Comma);
                consume();
            }
        }
        consume();

        expectToken(lexing::TokenType::Semicolon);
        consume();

        mGlobalSymbols[name] = Symbol(true, type, name);
        std::cout << name << "\n";

        return std::make_unique<ExternFunction>(type, name, std::move(arguments));
    }

    ReturnStatementPtr Parser::parseReturnStatement()
    {
        consume();

        if (current().getTokenType() == lexing::TokenType::Semicolon)
        {
            return std::make_unique<ReturnStatement>(nullptr);
        }

        return std::make_unique<ReturnStatement>(parseExpression());
    }

    VariableDeclarationPtr Parser::parseVariableDeclaration()
    {
        Type* type = parseType();

        expectToken(lexing::TokenType::Identifier);
        std::string name = consume().getText();

        if (current().getTokenType() == lexing::TokenType::Semicolon)
        {
            return std::make_unique<VariableDeclaration>(type, std::move(name), nullptr);
        }

        expectToken(lexing::TokenType::Equals);
        consume();

        ASTNodePtr initVal = parseExpression();

        mScope->symbols[name] = Symbol(true, type, name);

        return std::make_unique<VariableDeclaration>(type, std::move(name), std::move(initVal));
    }

    IfStatementPtr Parser::parseIfStatement()
    {
        consume(); // IfKeyword

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

            consume(); // ElseKeyword

            elseBody = parseExpression();
        }

        return std::make_unique<IfStatement>(std::move(condition), std::move(body), std::move(elseBody));
    }

    CompoundStatementPtr Parser::parseCompoundStatement()
    {
        consume(); // LeftBracket

        Environment* outerScope = mScope;
        Environment* scope = new Environment;
        scope->parent = outerScope;
        mScope = scope;

        std::vector<ASTNodePtr> body;
        while (current().getTokenType() != lexing::TokenType::RightBracket)
        {
            body.push_back(parseExpression());
            expectToken(lexing::TokenType::Semicolon);
            consume();
        }
        consume();

        mScope = outerScope;

        mTokens.insert(mTokens.begin() + mPosition, lexing::Token(lexing::TokenType::Semicolon));

        return std::make_unique<CompoundStatement>(std::move(body), scope);
    }

    IntegerLiteralPtr Parser::parseIntegerLiteral()
    {
        return std::make_unique<IntegerLiteral>(std::stoll(consume().getText()));
    }
    
    VariablePtr Parser::parseVariable()
    {
        std::string name = consume().getText();

        std::optional<Symbol> symbol = mScope->findSymbol(name);
        if (!symbol)
        {
            std::cout << name << "\n";
            symbol = mGlobalSymbols.at(name);
        }

        return std::make_unique<Variable>(name, symbol->getType());
    }

    CallExpressionPtr Parser::parseCallExpression(ASTNodePtr callee)
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

        return std::make_unique<CallExpression>(std::move(callee), std::move(parameters));
    }
}