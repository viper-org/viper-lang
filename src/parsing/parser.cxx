#include <asm-generic/errno-base.h>
#include <parsing/parser.hxx>
#include <diagnostics.hxx>
#include <iostream>

namespace Viper
{
    namespace Parsing
    {
        Parser::Parser(const std::vector<Lexing::Token>& tokens, const std::string& text)
            :_text(text), _tokens(tokens), _position(0)
        {
        }

        Lexing::Token Parser::Current() const
        {
            return _tokens[_position];
        }

        Lexing::Token Parser::Consume()
        {
            return _tokens[_position++];
        }

        Lexing::Token Parser::Peek(int offset) const
        {
            return _tokens[_position + offset];
        }

        std::string Parser::GetTokenText(Lexing::Token token) const
        {
            return _text.substr(token.getStart(), token.getEnd() - token.getStart());
        }

        void Parser::ExpectToken(Lexing::TokenType tokenType)
        {
            if(Current().getType() != tokenType)
            {
                Lexing::Token temp(tokenType, 0, 0, 0, 0);
                
                unsigned int start = Current().getStart();
                while(_text[start - 1] != '\n')
                    start--;
                unsigned int end = Current().getEnd();
                while(_text[end] != '\n')
                    end++;
                Diagnostics::CompilerError(Current().getLineNumber(), Current().getColNumber(), "Expected '" + temp.typeAsString() + "', found " + GetTokenText(Current()), 
                &_text[Current().getStart()], &_text[Current().getEnd()],
                &_text[start], &_text[end]);
            }
        }

        std::vector<std::unique_ptr<ASTTopLevel>> Parser::Parse()
        {
            std::vector<std::unique_ptr<ASTTopLevel>> nodes;
            while(Current().getType() != Lexing::TokenType::EndOfFile)
            {
                nodes.push_back(ParseFunction());
            }
            return nodes;
        }

        std::unique_ptr<ASTTopLevel> Parser::ParseFunction()
        {
            ExpectToken(Lexing::TokenType::Asperand);
            Consume();

            ExpectToken(Lexing::TokenType::Identifier);
            std::string name = GetTokenText(Consume());

            ExpectToken(Lexing::TokenType::LeftParen);
            Consume();
            // TODO: Parse args
            ExpectToken(Lexing::TokenType::RightParen);
            Consume();

            ExpectToken(Lexing::TokenType::RightArrow);
            Consume();

            ExpectToken(Lexing::TokenType::Identifier);
            Consume(); // TODO: Parse type

            ExpectToken(Lexing::TokenType::LeftBracket);
            Consume();

            std::vector<std::unique_ptr<ASTNode>> body;
            
            while(Current().getType() != Lexing::TokenType::RightBracket)
            {
                body.push_back(ParseExpression());
                ExpectToken(Lexing::TokenType::Semicolon);
                Consume();
            }
            Consume();

            return std::make_unique<ASTFunction>(name, std::move(body));
        }

        std::unique_ptr<ASTNode> Parser::ParseExpression()
        {
            // TODO: Parse unary/binary expressions
            return ParsePrimary();
        }

        std::unique_ptr<ASTNode> Parser::ParsePrimary()
        {
            switch(Current().getType())
            {
                case Lexing::TokenType::Integer:
                    return ParseInteger();
                case Lexing::TokenType::Identifier:
                    return ParseKeywordExpression();
                default:
                    // TODO: Compiler error
                    throw;
            }
        }

        std::unique_ptr<ASTNode> Parser::ParseInteger()
        {
            int value = std::stoi(GetTokenText(Consume()));

            return std::make_unique<IntegerLiteral>(value);
        }

        std::unique_ptr<ASTNode> Parser::ParseKeywordExpression()
        {
            if(GetTokenText(Current()) == "return")
            {
                Consume();
                if(Current().getType() == Lexing::TokenType::Semicolon)
                    return std::make_unique<ReturnStatement>(nullptr);
                return std::make_unique<ReturnStatement>(ParseExpression());
            }
            throw; // TODO: Compiler error
        }
    }
}