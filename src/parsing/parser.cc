#include <parsing/parser.hh>
#include <diagnostics.hh>

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

    Lexing::Token Parser::Peek(const int offset) const
    {
        return _tokens[_position + offset];
    }

    void Parser::ExpectToken(Lexing::TokenType tokenType)
    {
        if(Current().GetType() != tokenType)
        {
            Lexing::Token temp(tokenType, "", 0, 0, 0, 0);

            ParserError("Expected '" + temp.TypeAsString() + "', found " + Current().GetText());
        }
    }

    void Parser::ParserError(std::string message)
    {
        unsigned int start = Current().GetStart();
        while(_text[start] != '\n')
            start--;
        unsigned int end = Current().GetEnd();
        while(_text[end] != '\n')
            end++;
        Diagnostics::CompilerError(Current().GetLine(), Current().GetCol(),
        message, &_text[Current().GetStart()], &_text[Current().GetEnd()],
                &_text[start], &_text[end]);
    }


    std::vector<std::unique_ptr<ASTNode>> Parser::Parse()
    {
        std::vector<std::unique_ptr<ASTNode>> result;
        while(_position < _tokens.size())
        {
            std::unique_ptr<ASTNode> expr = ParseExpression();
            ExpectToken(Lexing::TokenType::Semicolon);
            Consume();

            if(expr->GetNodeType() == ASTNodeType::Function)
                result.push_back(std::move(expr));
            else; // TODO: Error
        }
        return result;
    }
    
    std::unique_ptr<ASTNode> Parser::ParseExpression(int)
    {
        return ParsePrimary();
    }

    std::unique_ptr<ASTNode> Parser::ParsePrimary()
    {
        switch(Current().GetType())
        {
            case Lexing::TokenType::Let:
                return ParseVariableDeclaration();
            case Lexing::TokenType::Return:
                return ParseReturnStatement();
            case Lexing::TokenType::Integer:
                return ParseIntegerLiteral();
            default:
                ParserError("Expected primary expression, found '" + Current().GetText() + "'");
        }
    }

    std::unique_ptr<ASTNode> Parser::ParseVariableDeclaration()
    {
        Consume();

        ExpectToken(Lexing::TokenType::Type);
        Consume();

        ExpectToken(Lexing::TokenType::Identifier);
        std::string name = Consume().GetText();

        bool isFunction = false;
        if(Current().GetType() == Lexing::TokenType::LeftParen)
        {
            Consume();
            isFunction = true;
            // TODO: Parse args
            ExpectToken(Lexing::TokenType::RightParen);
            Consume();
        }

        if(Current().GetType() != Lexing::TokenType::Equals)
            return std::make_unique<VariableDeclaration>(name, nullptr, isFunction);

        Consume();
        
        return std::make_unique<VariableDeclaration>(name, ParseExpression(), isFunction);
    }

    std::unique_ptr<ASTNode> Parser::ParseIntegerLiteral()
    {
        long long value = std::stoll(Consume().GetText());

        return std::make_unique<IntegerLiteral>(value);
    }

    std::unique_ptr<ASTNode> Parser::ParseReturnStatement()
    {
        Consume();

        if(Current().GetType() == Lexing::TokenType::Semicolon)
            return std::make_unique<ReturnStatement>(nullptr);

        return std::make_unique<ReturnStatement>(ParseExpression());
    }
}