#include <parsing/parser.hxx>
#include <diagnostics.hxx>

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

        int Parser::GetBinOpPrecedence(Lexing::TokenType tokenType) const
        {
            switch(tokenType)
            {
                case Lexing::TokenType::Star:
                case Lexing::TokenType::Slash:
                    return 40;
                case Lexing::TokenType::Plus:
                case Lexing::TokenType::Minus:
                    return 35;
                case Lexing::TokenType::DoubleEquals:
                case Lexing::TokenType::BangEquals:
                    return 25;
                case Lexing::TokenType::DoubleAmpersand:
                    return 15;
                case Lexing::TokenType::DoublePipe:
                    return 10;
                case Lexing::TokenType::Equals:
                    return 10;
                default:
                    return 0;
            }
        }

        void Parser::ExpectToken(Lexing::TokenType tokenType)
        {
            if(Current().getType() != tokenType)
            {
                Lexing::Token temp(tokenType, 0, 0, 0, 0);

                ParserError("Expected '" + temp.typeAsString() + "', found " + GetTokenText(Current()));
            }
        }

        void Parser::ParserError(std::string message)
        {
            unsigned int start = Current().getStart();
            while(_text[start] != '\n')
                start--;
            unsigned int end = Current().getEnd();
            while(_text[end] != '\n')
                end++;

            Diagnostics::CompilerError(Current().getLineNumber(), Current().getColNumber(),
            message, &_text[Current().getStart()], &_text[Current().getEnd()],
                      &_text[start], &_text[end]);
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

            std::shared_ptr<Type> type = ParseType();

            ExpectToken(Lexing::TokenType::LeftBracket);
            Consume();

            std::shared_ptr<Environment> scope = std::make_shared<Environment>();
            _currentScope = scope;

            std::vector<std::unique_ptr<ASTNode>> body;
            
            while(Current().getType() != Lexing::TokenType::RightBracket)
            {
                body.push_back(ParseExpression());
                ExpectToken(Lexing::TokenType::Semicolon);
                Consume();
            }
            Consume();

            return std::make_unique<ASTFunction>(name, type, std::move(body), scope);
        }

        std::shared_ptr<Type> Parser::ParseType()
        {
            ExpectToken(Lexing::TokenType::Type);
            std::string text = GetTokenText(Consume());
            if(auto iterator = types.find(text); iterator != types.end())
                return iterator->second;

            ParserError("Expected type, found '" + GetTokenText(Current()) + "'");
        }

        std::unique_ptr<ASTNode> Parser::ParseExpression(int precedence)
        {
            std::unique_ptr<ASTNode> lhs = ParsePrimary();

            while(true)
            {
                int binOpPrecedence = GetBinOpPrecedence(Current().getType());
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
            switch(Current().getType())
            {
                case Lexing::TokenType::Integer:
                    return ParseInteger();
                case Lexing::TokenType::Return:
                    return ParseReturn();
                case Lexing::TokenType::If:
                    return ParseIfStatement();
                case Lexing::TokenType::While:
                    return ParseWhileStatement();
                case Lexing::TokenType::Break:
                    return ParseBreakStatement();
                case Lexing::TokenType::LeftParen:
                    return ParseParenthesizedExpression();
                case Lexing::TokenType::Type:
                    return ParseVariableDeclaration();
                case Lexing::TokenType::Identifier:
                    return ParseVariable();
                case Lexing::TokenType::LeftBracket:
                    return ParseCompoundStatement();
                default:
                    ParserError("Expected primary expression, found '" + GetTokenText(Current()) + "'");
            }
        }

        std::unique_ptr<ASTNode> Parser::ParseInteger()
        {
            int value = std::stoi(GetTokenText(Consume()));

            return std::make_unique<IntegerLiteral>(value);
        }

        std::unique_ptr<ASTNode> Parser::ParseReturn()
        {
            Consume();

            if(Current().getType() == Lexing::TokenType::Semicolon)
                return std::make_unique<ReturnStatement>(nullptr);

            return std::make_unique<ReturnStatement>(ParseExpression());
        }

        std::unique_ptr<ASTNode> Parser::ParseParenthesizedExpression()
        {
            Consume();

            std::unique_ptr<ASTNode> expression = ParseExpression();

            ExpectToken(Lexing::TokenType::RightParen);
            Consume();

            return expression;
        }

        std::unique_ptr<ASTNode> Parser::ParseVariableDeclaration()
        {
            std::shared_ptr<Type> type = ParseType();

            ExpectToken(Lexing::TokenType::Identifier);
            std::string name = GetTokenText(Consume());

            if(Current().getType() != Lexing::TokenType::Equals)
                return std::make_unique<VariableDeclaration>(type, name, nullptr);
            
            Consume();

            return std::make_unique<VariableDeclaration>(type, name, ParseExpression());
        }

        std::unique_ptr<ASTNode> Parser::ParseVariable()
        {
            return std::make_unique<Variable>(GetTokenText(Consume()));
        }

        std::unique_ptr<ASTNode> Parser::ParseIfStatement()
        {
            Consume();

            ExpectToken(Lexing::TokenType::LeftParen);
            Consume();
            std::unique_ptr<ASTNode> cond = ParseExpression();
            ExpectToken(Lexing::TokenType::RightParen);
            Consume();

            std::shared_ptr<Environment> scope = std::make_shared<Environment>();
            scope->outer = _currentScope;
            _currentScope = scope;

            std::unique_ptr<ASTNode> body = ParseExpression();

            _currentScope = _currentScope->outer;

            if(GetTokenText(Peek(1)) != "else")
                return std::make_unique<IfStatement>(std::move(cond), scope, std::move(body), nullptr, nullptr);

            ExpectToken(Lexing::TokenType::Semicolon);
            Consume();
            
            Consume();

            std::shared_ptr<Environment> elseScope = std::make_shared<Environment>();
            elseScope->outer = _currentScope;
            _currentScope = elseScope;

            std::unique_ptr<ASTNode> elseBody = ParseExpression();

            _currentScope = _currentScope->outer;

            return std::make_unique<IfStatement>(std::move(cond), scope, std::move(body), elseScope, std::move(elseBody));
        }

        std::unique_ptr<ASTNode> Parser::ParseWhileStatement()
        {
            Consume();

            ExpectToken(Lexing::TokenType::LeftParen);
            Consume();

            std::unique_ptr<ASTNode> condition = ParseExpression();

            ExpectToken(Lexing::TokenType::RightParen);
            Consume();

            std::shared_ptr<Environment> scope = std::make_shared<Environment>();
            scope->outer = _currentScope;
            _currentScope = scope;

            std::unique_ptr<ASTNode> body = ParseExpression();

            _currentScope = scope->outer;

            return std::make_unique<WhileStatement>(std::move(condition), std::move(body), scope);
        }

        std::unique_ptr<ASTNode> Parser::ParseBreakStatement()
        {
            Consume();

            return std::make_unique<BreakStatement>();
        }

        std::unique_ptr<ASTNode> Parser::ParseCompoundStatement()
        {
            Consume();

            std::shared_ptr<Environment> scope = std::make_shared<Environment>();
            scope->outer = _currentScope;
            _currentScope = scope;

            std::vector<std::unique_ptr<ASTNode>> statements;

            while(Current().getType() != Lexing::TokenType::RightBracket)
            {
                statements.push_back(ParseExpression());
                ExpectToken(Lexing::TokenType::Semicolon);
                Consume();
            }
            Consume();
            _tokens.insert(_tokens.begin() + _position, Lexing::Token(Lexing::TokenType::Semicolon, 0, 0, 0, 0));

            _currentScope = scope->outer;

            return std::make_unique<CompoundStatement>(std::move(statements), scope);
        }
    }
}