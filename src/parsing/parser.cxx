#include "lexing/token.hxx"
#include <iostream>
#include <parsing/parser.hxx>
#include <diagnostics.hxx>
#include <sstream>
#include <algorithm>

namespace Quark
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
            return _text.substr(token.getStart(), token.getEnd() - token.getStart() + 1);
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
                while(_text[end + 1] != '\n')
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
            // TODO: Parse body
            ExpectToken(Lexing::TokenType::RightBracket);
            Consume();

            return std::make_unique<ASTFunction>(name, std::vector<std::unique_ptr<ASTNode>>());
        }
    }
}