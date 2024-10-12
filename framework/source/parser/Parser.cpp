// Copyright 2024 solar-mist

#include "parser/Parser.h"

#include <cinttypes>
#include <format>

namespace parser
{
    Parser::Parser(std::vector<lexer::Token>& tokens, diagnostic::Diagnostics& diag, Scope* globalScope)
        : mTokens(tokens)
        , mPosition(0)
        , mDiag(diag)
        , mActiveScope(globalScope)
    {
    }

    std::vector<ASTNodePtr> Parser::parse()
    {
        std::vector<ASTNodePtr> ast;

        while (mPosition < mTokens.size())
        {
            ast.push_back(parseGlobal());
        }

        return ast;
    }

    lexer::Token Parser::current() const
    {
        return mTokens[mPosition];
    }

    lexer::Token Parser::consume()
    {
        return mTokens[mPosition++];
    }

    lexer::Token Parser::peek(int offset) const
    {
        return mTokens[mPosition + offset];
    }

    void Parser::expectToken(lexer::TokenType tokenType)
    {
        if (current().getTokenType() != tokenType)
        {
            lexer::Token temp("", tokenType, lexer::SourceLocation(), lexer::SourceLocation());
            mDiag.reportCompilerError(
                current().getStartLocation(),
                current().getEndLocation(),
                std::format("Expected '{}{}{}', found '{}{}{}'",
                    fmt::bold, temp.getName(), fmt::defaults,
                    fmt::bold, current().getText(), fmt::defaults)
            );
            std::exit(1);
        }
    }


    ASTNodePtr Parser::parseGlobal()
    {
        switch (current().getTokenType())
        {
            case lexer::TokenType::FuncKeyword:
                return parseFunction();

            default:
                mDiag.reportCompilerError(
                    current().getStartLocation(),
                    current().getEndLocation(),
                    std::format("Expected global expression. Found '{}{}{}'", fmt::bold, current().getText(), fmt::defaults)
                );
                std::exit(1);
                return nullptr;
        }
    }

    ASTNodePtr Parser::parseExpression()
    {
        // TODO: Binary and unary expression parsing
        return parsePrimary();
    }

    ASTNodePtr Parser::parsePrimary()
    {
        switch (current().getTokenType())
        {
            case lexer::TokenType::ReturnKeyword:
                return parseReturnStatement();

            case lexer::TokenType::IntegerLiteral:
                return parseIntegerLiteral();

            default:
                mDiag.reportCompilerError(
                    current().getStartLocation(),
                    current().getEndLocation(),
                    std::format("Expected primary expression. Found '{}{}{}'", fmt::bold, current().getText(), fmt::defaults)
                );
                std::exit(1);
        }
    }


    FunctionPtr Parser::parseFunction()
    {
        consume(); // FuncKeyword

        expectToken(lexer::TokenType::Identifier);
        std::string name = std::string(consume().getText());

        expectToken(lexer::TokenType::LeftParen);
        consume();
        // TODO: Parse arguments
        expectToken(lexer::TokenType::RightParen);
        consume();

        expectToken(lexer::TokenType::RightArrow);
        consume();
        // TODO: Parse type
        expectToken(lexer::TokenType::I32Keyword);
        consume();

        ScopePtr scope = std::make_unique<Scope>(mActiveScope, "", false);
        mActiveScope = scope.get();

        std::vector<ASTNodePtr> body;
        expectToken(lexer::TokenType::LeftBrace);
        consume();

        while (current().getTokenType() != lexer::TokenType::RightBrace)
        {
            body.push_back(parseExpression());
            expectToken(lexer::TokenType::Semicolon);
            consume();
        }
        consume();

        mActiveScope = scope->parent;

        return std::make_unique<Function>(std::move(name), std::move(body), std::move(scope));
    }


    ReturnStatementPtr Parser::parseReturnStatement()
    {
        consume(); // ReturnKeyword

        if (current().getTokenType() == lexer::TokenType::Semicolon)
        {
            return std::make_unique<ReturnStatement>(mActiveScope, nullptr);
        }

        return std::make_unique<ReturnStatement>(mActiveScope, parseExpression());
    }


    IntegerLiteralPtr Parser::parseIntegerLiteral()
    {
        std::string text = std::string(consume().getText());

        return std::make_unique<IntegerLiteral>(mActiveScope, std::strtoimax(text.c_str(), nullptr, 0));
    }
}