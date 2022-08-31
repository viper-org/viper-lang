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

            case Lexing::TokenType::Equals:
                return 10;

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

    std::vector<std::unique_ptr<ASTTopLevel>> Parser::Parse()
    {
        std::vector<std::unique_ptr<ASTTopLevel>> result;
        while(_position < _tokens.size())
        {
            result.push_back(ParseTopLevel());
        }
        return result;
    }

    std::unique_ptr<ASTTopLevel> Parser::ParseTopLevel()
    {
        switch(Current().GetType())
        {
            case Lexing::TokenType::Asperand:
                return ParseFunction();
            case Lexing::TokenType::Extern:
                return ParseExtern();
            default:
                ParserError("Expected top-level expression, found '" + Current().GetText() + "'");
        }
    }

    std::unique_ptr<ASTTopLevel> Parser::ParseFunction()
    {
        Consume();

        ExpectToken(Lexing::TokenType::Identifier);
        std::string name = Consume().GetText();

        ExpectToken(Lexing::TokenType::LeftParen);
        Consume();
        std::vector<std::string> args;
        while(Current().GetType() != Lexing::TokenType::RightParen)
        {
            ExpectToken(Lexing::TokenType::Type);
            Consume();

            args.push_back(Consume().GetText());
            if(Current().GetType() == Lexing::TokenType::RightParen)
                break;

            ExpectToken(Lexing::TokenType::Comma);
            Consume();
        }
        Consume();

        ExpectToken(Lexing::TokenType::RightArrow);
        Consume();

        Type* type = ParseType();

        ExpectToken(Lexing::TokenType::LeftBracket);
        Consume();

        std::vector<std::unique_ptr<ASTNode>> body;
        while(Current().GetType() != Lexing::TokenType::RightBracket)
        {
            body.push_back(ParseExpression());
            ExpectToken(Lexing::TokenType::Semicolon);
            Consume();
        }
        Consume();

        return std::make_unique<ASTFunction>(name, args, std::move(body), type);
    }

    std::unique_ptr<ASTTopLevel> Parser::ParseExtern()
    {
        Consume();
        ExpectToken(Lexing::TokenType::Asperand);
        Consume();
        
        ExpectToken(Lexing::TokenType::Identifier);
        std::string name = Consume().GetText();

        ExpectToken(Lexing::TokenType::LeftParen);
        Consume();
        std::vector<std::string> args;
        while(Current().GetType() != Lexing::TokenType::RightParen)
        {
            ExpectToken(Lexing::TokenType::Type);
            Consume();

            args.push_back(Consume().GetText());
            if(Current().GetType() == Lexing::TokenType::RightParen)
                break;

            ExpectToken(Lexing::TokenType::Comma);
            Consume();
        }
        Consume();

        ExpectToken(Lexing::TokenType::RightArrow);
        Consume();

        Type* type = ParseType();

        ExpectToken(Lexing::TokenType::Semicolon);
        Consume();

        return std::make_unique<ASTFunction>(name, args, std::vector<std::unique_ptr<ASTNode>>(), type);
    }

    Type* Parser::ParseType()
    {
        return types.at(Consume().GetText());
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
            case Lexing::TokenType::Integer:
                return ParseIntegerLiteral();
            case Lexing::TokenType::Return:
                return ParseReturnStatement();
            case Lexing::TokenType::Type:
                return ParseVariableDeclaration();
            case Lexing::TokenType::Identifier:
            {
                if(Peek(1).GetType() == Lexing::TokenType::LeftParen)
                    return ParseCall();
                return ParseVariable();
            }
            default:
                ParserError("Expected primary expression, found '" + Current().GetText() + "'");
        }
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

    std::unique_ptr<ASTNode> Parser::ParseVariableDeclaration()
    {
        Type* type = ParseType();
        std::string name = Consume().GetText();

        if(Current().GetType() != Lexing::TokenType::Equals)
            return std::make_unique<VariableDeclaration>(type, name, nullptr);

        Consume();

        return std::make_unique<VariableDeclaration>(type, name, ParseExpression());
    }

    std::unique_ptr<ASTNode> Parser::ParseVariable()
    {
        return std::make_unique<Variable>(Consume().GetText());
    }

    std::unique_ptr<ASTNode> Parser::ParseCall()
    {
        std::string callee = Consume().GetText();

        Consume();
        std::vector<std::unique_ptr<ASTNode>> args;
        while(Current().GetType() != Lexing::TokenType::RightParen)
        {
            args.push_back(ParseExpression());
            if(Current().GetType() == Lexing::TokenType::RightParen)
                break;

            ExpectToken(Lexing::TokenType::Comma);
            Consume();
        }
        Consume();

        return std::make_unique<CallExpr>(callee, std::move(args));
    }
}