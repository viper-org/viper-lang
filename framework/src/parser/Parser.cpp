// Copyright 2023 solar-mist


#include "parser/Parser.h"

#include "parser/ast/expression/BinaryExpression.h"

#include <iostream>

namespace parsing
{
    Parser::Parser(std::vector<lexing::Token>& tokens)
        : mTokens(tokens)
        , mPosition(0)
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
            case lexing::TokenType::Plus:
            case lexing::TokenType::Minus:
                return 35;
            default:
                return 0;
        }
    }

    std::vector<ASTNodePtr> Parser::parse()
    {
        std::vector<ASTNodePtr> result;

        while (mPosition < mTokens.size())
        {
            result.push_back(parseFunction());
        }

        return result;
    }

    ASTNodePtr Parser::parseExpression(int precedence)
    {
        ASTNodePtr lhs = parsePrimary();

        while (true)
        {
            int binaryOperatorPrecedence = getBinaryOperatorPrecedence(current().getTokenType());
            if (binaryOperatorPrecedence < precedence)
            {
                break;
            }
            
            lexing::Token operatorToken = consume();
            ASTNodePtr rhs = parsePrimary();
            lhs = std::make_unique<BinaryExpression>(std::move(lhs), operatorToken.getTokenType(), std::move(rhs));
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
            default:
                std::cerr << "Unexpected token. Expected primary expression.\n";
                std::exit(1);
        }
    }

    FunctionPtr Parser::parseFunction()
    {
        expectToken(lexing::TokenType::Type);
        consume();

        expectToken(lexing::TokenType::Identifier);
        std::string name = consume().getText();

        expectToken(lexing::TokenType::LeftParen);
        consume();
        // TODO: Parse arguments
        expectToken(lexing::TokenType::RightParen);
        consume();

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

        return std::make_unique<Function>(name, std::move(body));
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
        consume(); // TODO: expectToken(lexing::TokenType::Type)

        expectToken(lexing::TokenType::Identifier);
        std::string name = consume().getText();

        if (current().getTokenType() == lexing::TokenType::Semicolon)
        {
            return std::make_unique<VariableDeclaration>(std::move(name), nullptr);
        }

        expectToken(lexing::TokenType::Equals);
        consume();

        return std::make_unique<VariableDeclaration>(std::move(name), parseExpression());
    }

    IntegerLiteralPtr Parser::parseIntegerLiteral()
    {
        return std::make_unique<IntegerLiteral>(std::stoll(consume().getText()));
    }
    
    VariablePtr Parser::parseVariable()
    {
        return std::make_unique<Variable>(consume().getText());
    }
}