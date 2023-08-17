// Copyright 2023 solar-mist


#include "parser/Parser.h"

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

    std::vector<ASTNodePtr> Parser::parse()
    {
        std::vector<ASTNodePtr> result;

        while (mPosition < mTokens.size())
        {
            result.push_back(parseFunction());
        }

        return result;
    }

    FunctionPtr Parser::parseFunction()
    {
        expectToken(lexing::TokenType::Identifier); // TODO: expectToken(lexing::TokenType::Type)
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
        // TODO: Parse body
        
        expectToken(lexing::TokenType::RightBracket);
        consume();

        return std::make_unique<Function>(name, std::move(body));
    }
}