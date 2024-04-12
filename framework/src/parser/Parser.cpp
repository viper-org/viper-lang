// Copyright 2023 solar-mist


#include "parser/Parser.h"

#include "lexer/Token.h"

#include <iostream>

namespace parser
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
            default:
                std::cerr << "Unexpected token: " << current().toString() << ". Expected global statement.\n";
                std::exit(1);
        }
    }
    
    Type* Parser::parseType()
    {
        expectToken(lexing::TokenType::Type);

        return Type::Get(consume().getText());
    }

    ASTNodePtr Parser::parseExpression(Type* preferredType)
    {
        return parsePrimary(preferredType);
    }

    ASTNodePtr Parser::parsePrimary(Type* preferredType)
    {
        switch (current().getTokenType())
        {
            case lexing::TokenType::ReturnKeyword:
                return parseReturnStatement();
            
            case lexing::TokenType::LetKeyword:
                return parseVariableDeclaration();

            case lexing::TokenType::IntegerLiteral:
                return parseIntegerLiteral(preferredType);

            case lexing::TokenType::Identifier:
                return parseVariableExpression(preferredType);

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
        expectToken(lexing::TokenType::RightParen);
        consume();

        expectToken(lexing::TokenType::RightArrow);
        consume();
        
        Type* type = parseType();

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

        return std::make_unique<Function>(type, name, std::move(body), functionScope);
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

        if (current().getTokenType() == lexing::TokenType::Semicolon)
        {
            return std::make_unique<VariableDeclaration>(type, std::move(name), nullptr);
        }

        expectToken(lexing::TokenType::Equals);
        consume();

        return std::make_unique<VariableDeclaration>(type, std::move(name), parseExpression(type));
    }

    IntegerLiteralPtr Parser::parseIntegerLiteral(Type* preferredType)
    {
        return std::make_unique<IntegerLiteral>(std::stoll(consume().getText()), preferredType);
    }

    VariableExpressionPtr Parser::parseVariableExpression(Type* preferredType)
    {
        std::string name = consume().getText();

        return std::make_unique<VariableExpression>(std::move(name));
    }
}