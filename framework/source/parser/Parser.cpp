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

    int Parser::getBinaryOperatorPrecedence(lexer::TokenType tokenType) 
    {
        switch (tokenType) 
        {
            case lexer::TokenType::Star:
            case lexer::TokenType::Slash:
                return 75;
            case lexer::TokenType::Plus:
            case lexer::TokenType::Minus:
                return 70;

            case lexer::TokenType::Equal:
                return 20;

            default:
                return 0;
        }
    }

    int Parser::getPrefixUnaryOperatorPrecedence(lexer::TokenType tokenType) 
    {
        switch (tokenType) 
        {
            case lexer::TokenType::Minus:
                return 85;
            default:
                return 0;
        }
    }

    int Parser::getPostfixUnaryOperatorPrecedence(lexer::TokenType tokenType) 
    {
        return 0;
    }

    Type* Parser::parseType()
    {
        expectToken(lexer::TokenType::TypeKeyword);
        return Type::Get(std::string(consume().getText()));
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

    ASTNodePtr Parser::parseExpression(int precedence)
    {
        ASTNodePtr left;
        int prefixOperatorPrecedence = getPrefixUnaryOperatorPrecedence(current().getTokenType());

        if (prefixOperatorPrecedence >= precedence)
        {
            lexer::Token operatorToken = consume();
            left = std::make_unique<UnaryExpression>(mActiveScope, parseExpression(prefixOperatorPrecedence), operatorToken.getTokenType(), false, std::move(operatorToken));
        }
        else
        {
            left = parsePrimary();
        }

        while (true)
        {
            int postfixOperatorPrecedence = getPostfixUnaryOperatorPrecedence(current().getTokenType());
            if (postfixOperatorPrecedence < precedence) 
            {
                break;
            }

            lexer::Token operatorToken = consume();

            left = std::make_unique<UnaryExpression>(mActiveScope, std::move(left), operatorToken.getTokenType(), true, std::move(operatorToken));
        }

        while (true) 
        {
            int binaryOperatorPrecedence = getBinaryOperatorPrecedence(current().getTokenType());
            if (binaryOperatorPrecedence < precedence) 
            {
                break;
            }

            lexer::Token operatorToken = consume();

            ASTNodePtr right = parseExpression(binaryOperatorPrecedence);
            left = std::make_unique<BinaryExpression>(mActiveScope, std::move(left), operatorToken.getTokenType(), std::move(right), std::move(operatorToken));
        }

        return left;
    }

    ASTNodePtr Parser::parsePrimary()
    {
        switch (current().getTokenType())
        {
            case lexer::TokenType::ReturnKeyword:
                return parseReturnStatement();

            case lexer::TokenType::LetKeyword:
                return parseVariableDeclaration();

            case lexer::TokenType::IntegerLiteral:
                return parseIntegerLiteral();

            case lexer::TokenType::Identifier:
                return parseVariableExpression();

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
        auto token = consume(); // FuncKeyword

        expectToken(lexer::TokenType::Identifier);
        std::string name = std::string(consume().getText());

        expectToken(lexer::TokenType::LeftParen);
        consume();
        // TODO: Parse arguments
        expectToken(lexer::TokenType::RightParen);
        consume();

        expectToken(lexer::TokenType::RightArrow);
        consume();
        Type* returnType = parseType();

        FunctionType* functionType = FunctionType::Create(returnType);

        ScopePtr scope = std::make_unique<Scope>(mActiveScope, "", false, returnType);
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

        return std::make_unique<Function>(std::move(name), functionType, std::move(body), std::move(scope), std::move(token));
    }


    ReturnStatementPtr Parser::parseReturnStatement()
    {
        auto token = consume(); // ReturnKeyword

        if (current().getTokenType() == lexer::TokenType::Semicolon)
        {
            return std::make_unique<ReturnStatement>(mActiveScope, nullptr, std::move(token));
        }

        return std::make_unique<ReturnStatement>(mActiveScope, parseExpression(), std::move(token));
    }

    VariableDeclarationPtr Parser::parseVariableDeclaration()
    {
        consume(); // LetKeyword

        expectToken(lexer::TokenType::Identifier);
        auto token = consume();
        std::string name = std::string(token.getText());
        
        expectToken(lexer::TokenType::Colon);
        consume();

        auto type = parseType();

        ASTNodePtr initValue = nullptr;
        if (current().getTokenType() == lexer::TokenType::Equal)
        {
            consume();

            initValue = parseExpression();
        }

        return std::make_unique<VariableDeclaration>(mActiveScope, std::move(name), type, std::move(initValue), std::move(token));
    }


    IntegerLiteralPtr Parser::parseIntegerLiteral()
    {
        auto token = consume();
        std::string text = std::string(token.getText());

        return std::make_unique<IntegerLiteral>(mActiveScope, std::strtoimax(text.c_str(), nullptr, 0), std::move(token));
    }

    VariableExpressionPtr Parser::parseVariableExpression()
    {
        auto token = consume();
        std::string text = std::string(token.getText());

        return std::make_unique<VariableExpression>(mActiveScope, std::move(text), std::move(token));
    }
}