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

    int Parser::GetBinOpPrecedence(Lexing::TokenType type)
    {
        switch(type)
        {
            case Lexing::TokenType::Star:
            case Lexing::TokenType::Slash:
                return 40;
            
            case Lexing::TokenType::Plus:
            case Lexing::TokenType::Minus:
                return 35;

            case Lexing::TokenType::LeftAngleBracket:
            case Lexing::TokenType::RightAngleBracket:
                return 30;
                
            case Lexing::TokenType::DoubleEquals:
            case Lexing::TokenType::BangEquals:
                return 25;            
        
            case Lexing::TokenType::Equals:
                return 10;
            
            default:
                return 0;
        }
    }

    int Parser::GetUnOpPrecedence(Lexing::TokenType type)
    {
        switch(type)
        {
            case Lexing::TokenType::Minus:
            case Lexing::TokenType::Hash:
            case Lexing::TokenType::Asperand:
                return 50;
            default:
                return 0;
        }
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
            int savePos = _position;
            std::unique_ptr<ASTNode> expr = ParseExpression();
            ExpectToken(Lexing::TokenType::Semicolon);
            Consume();

            if(expr->GetNodeType() == ASTNodeType::Function)
                result.push_back(std::move(expr));
            else
            {
                _position = savePos;
                ParserError("Expected top-level expression");
            }
        }
        return result;
    }
    
    std::unique_ptr<ASTNode> Parser::ParseExpression(int precedence)
    {
        std::unique_ptr<ASTNode> lhs = ParsePrimary();

        while(true)
        {
            int binOpPrecedence = GetBinOpPrecedence(Current().GetType());
            if(binOpPrecedence < precedence)
                break;

            Lexing::Token operatorToken = Consume();
            std::unique_ptr<ASTNode> rhs = ParseExpression(binOpPrecedence);
            lhs = std::make_unique<BinaryExpression>(std::move(lhs), operatorToken, std::move(rhs));
        }

        return lhs;
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
            case Lexing::TokenType::LeftBracket:
                return ParseCompoundExpression();
            case Lexing::TokenType::LeftParen:
                return ParseParenthesizedExpression();
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

        ExpectToken(Lexing::TokenType::LeftParen);
        Consume();
        // TODO: Parse args
        ExpectToken(Lexing::TokenType::RightParen);
        Consume();

        ExpectToken(Lexing::TokenType::Equals);
        Consume();
        
        std::unique_ptr<ASTNode> initVal = ParseExpression();

        return std::make_unique<VariableDeclaration>(name, std::move(initVal), true);
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

    std::unique_ptr<ASTNode> Parser::ParseCompoundExpression()
    {
        Consume();

        std::vector<std::unique_ptr<ASTNode>> exprs;

        while(Current().GetType() != Lexing::TokenType::RightBracket)
        {
            exprs.push_back(ParseExpression());
            ExpectToken(Lexing::TokenType::Semicolon);
            Consume();
        }
        Consume();

        _tokens.insert(_tokens.begin() + _position, Lexing::Token(Lexing::TokenType::Semicolon, "", 0, 0, 0, 0));

        return std::make_unique<CompoundStatement>(exprs);
    }

    std::unique_ptr<ASTNode> Parser::ParseParenthesizedExpression()
    {
        Consume();

        std::unique_ptr<ASTNode> expr = ParseExpression();

        ExpectToken(Lexing::TokenType::RightParen);
        Consume();

        return expr;
    }
}