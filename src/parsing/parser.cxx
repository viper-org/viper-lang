#include <iostream>
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

                case Lexing::TokenType::LessThan:
                case Lexing::TokenType::LessEquals:
                case Lexing::TokenType::GreaterThan:
                case Lexing::TokenType::GreaterEquals:
                    return 30;

                case Lexing::TokenType::DoubleEquals:
                case Lexing::TokenType::BangEquals:
                    return 25;

                case Lexing::TokenType::DoubleAmpersand:
                    return 20;
                case Lexing::TokenType::DoublePipe:
                    return 15;
                
                case Lexing::TokenType::Equals:
                    return 10;
                default:
                    return 0;
            }
        }

        int Parser::GetPrefixUnOpPrecedence(Lexing::TokenType tokenType) const
        {
            switch(tokenType)
            {
                case Lexing::TokenType::Bang:
                case Lexing::TokenType::Minus:
                case Lexing::TokenType::Increment:
                case Lexing::TokenType::Decrement:
                    return 50;
                default:
                    return 0;
            }
        }

        int Parser::GetPostfixUnOpPrecedence(Lexing::TokenType tokenType) const
        {
            switch(tokenType)
            {
                case Lexing::TokenType::Increment:
                case Lexing::TokenType::Decrement:
                    return 55;
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
                nodes.push_back(ParseTopLevel());
            }
            return nodes;
        }

        std::unique_ptr<ASTTopLevel> Parser::ParseTopLevel()
        {
            switch(Current().getType())
            {
                case Lexing::TokenType::Asperand:
                    return ParseFunction();
                case Lexing::TokenType::Extern:
                    return ParseExtern();
                default:
                    ParserError("Expected top-level expression, found '" + GetTokenText(Current()) + "'");
            }
        }

        std::unique_ptr<ASTTopLevel> Parser::ParseExtern()
        {
            Consume();

            ExpectToken(Lexing::TokenType::Asperand);
            Consume();

            std::string name = GetTokenText(Consume());

            ExpectToken(Lexing::TokenType::LeftParen);
            Consume();

            std::vector<std::pair<std::shared_ptr<Type>, std::string>> args;
            while(Current().getType() != Lexing::TokenType::RightParen)
            {
                std::shared_ptr<Type> type = ParseType();
                args.push_back(std::make_pair(type, GetTokenText(Consume())));
                if(Current().getType() == Lexing::TokenType::RightParen)
                    break;
                ExpectToken(Lexing::TokenType::Comma);
                Consume();
            }
            Consume();

            ExpectToken(Lexing::TokenType::RightArrow);
            Consume();

            std::shared_ptr<Type> type = ParseType();

            ExpectToken(Lexing::TokenType::Semicolon);
            Consume();

            return std::make_unique<ExternFunction>(name, type, args);
        }

        std::unique_ptr<ASTTopLevel> Parser::ParseFunction()
        {
            identifiers.clear();
            ExpectToken(Lexing::TokenType::Asperand);
            Consume();

            ExpectToken(Lexing::TokenType::Identifier);
            std::string name = GetTokenText(Consume());

            ExpectToken(Lexing::TokenType::LeftParen);
            Consume();
            
            std::vector<std::pair<std::shared_ptr<Type>, std::string>> args;
            while(Current().getType() != Lexing::TokenType::RightParen)
            {
                std::shared_ptr<Type> type = ParseType();
                std::string name = GetTokenText(Consume());
                identifiers.push_back(name);
                args.push_back(std::make_pair(type, name));

                if(Current().getType() == Lexing::TokenType::RightParen)
                    break;

                ExpectToken(Lexing::TokenType::Comma);
                Consume();
            }
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

            return std::make_unique<ASTFunction>(name, type, std::move(args), std::move(body), scope);
        }

        std::shared_ptr<Type> Parser::ParseType()
        {
            ExpectToken(Lexing::TokenType::Type);
            std::string text = GetTokenText(Consume());
            if(auto iterator = types.find(text); iterator != types.end())
            {
                std::shared_ptr<Type> type = iterator->second;
                while(Current().getType() == Lexing::TokenType::LeftSquareBracket || Current().getType() == Lexing::TokenType::Star)
                {
                    if(Current().getType() == Lexing::TokenType::Star)
                    {
                        Consume();
                        type = std::make_shared<PointerType>(type);
                    }
                    else
                    {
                        Consume();
                        unsigned int length = std::stoi(GetTokenText(Consume()));

                        ExpectToken(Lexing::TokenType::RightSquareBracket);
                        Consume();

                        type = std::make_shared<ArrayType>(length, type);
                    }
                }
                return type;
            }

            ParserError("Expected type, found '" + GetTokenText(Current()) + "'");
        }

        std::unique_ptr<ASTNode> Parser::ParseExpression(int precedence)
        {
            std::unique_ptr<ASTNode> lhs;
            int unOpPrecedence = GetPrefixUnOpPrecedence(Current().getType());
            if(unOpPrecedence != 0 && unOpPrecedence >= precedence)
            {
                Lexing::Token operatorToken = Consume();
                std::unique_ptr<ASTNode> operand = ParseExpression(unOpPrecedence);
                lhs = std::make_unique<UnaryExpression>(std::move(operand), operatorToken);
            }
            else
                lhs = ParsePrimary();

            while(true)
            {
                int binOpPrecedence = GetBinOpPrecedence(Current().getType());
                if(binOpPrecedence < precedence)
                    break;

                Lexing::Token operatorToken = Consume();
                std::unique_ptr<ASTNode> rhs = ParseExpression(binOpPrecedence);
                lhs = std::make_unique<BinaryExpression>(std::move(lhs), operatorToken, std::move(rhs));
            }
            unOpPrecedence = GetPostfixUnOpPrecedence(Current().getType());
            if(unOpPrecedence != 0 && unOpPrecedence >= precedence)
            {
                Lexing::Token operatorToken = Consume();
                switch(operatorToken.getType())
                {
                    case Lexing::TokenType::Increment:
                        lhs = std::make_unique<UnaryExpression>(std::move(lhs), UnaryOperator::PostfixIncrement);
                        break;
                    case Lexing::TokenType::Decrement:
                        lhs = std::make_unique<UnaryExpression>(std::move(lhs), UnaryOperator::PostfixDecrement);
                        break;
                    default: // This should never be reached
                        break;
                }
            }
            return lhs;
        }

        std::unique_ptr<ASTNode> Parser::ParsePrimary()
        {
            switch(Current().getType())
            {
                case Lexing::TokenType::Integer:
                    return ParseInteger();
                case Lexing::TokenType::Character:
                    return ParseCharacter();
                case Lexing::TokenType::String:
                    return ParseString();
                
                case Lexing::TokenType::Return:
                    return ParseReturn();
                case Lexing::TokenType::If:
                    return ParseIfStatement();
                case Lexing::TokenType::While:
                    return ParseWhileStatement();
                case Lexing::TokenType::For:
                    return ParseForStatement();
                case Lexing::TokenType::Break:
                    return ParseBreakStatement();
                case Lexing::TokenType::LeftParen:
                    return ParseParenthesizedExpression();
                
                case Lexing::TokenType::Type:
                    return ParseVariableDeclaration();
                case Lexing::TokenType::Identifier:
                {
                    if(Peek(1).getType() == Lexing::TokenType::LeftParen)
                        return ParseCallExpression();
                    if(Peek(1).getType() == Lexing::TokenType::LeftSquareBracket)
                        return ParseSubscript();
                    return ParseVariable();
                }

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

        std::unique_ptr<ASTNode> Parser::ParseCharacter()
        {
            char ch = GetTokenText(Consume())[0];
            
            return std::make_unique<IntegerLiteral>((int)ch);
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

            identifiers.push_back(name);

            if(Current().getType() != Lexing::TokenType::Equals)
                return std::make_unique<VariableDeclaration>(type, name, nullptr);
            
            Consume();

            return std::make_unique<VariableDeclaration>(type, name, ParseExpression());
        }

        std::unique_ptr<ASTNode> Parser::ParseVariable()
        {
            std::string name = GetTokenText(Consume());
            if(std::find(identifiers.begin(), identifiers.end(), name) == identifiers.end())
            {
                _position--;
                ParserError("'\x1b[1m" + name + "\x1b[0m' undeclared (first use in this function)");
            }
            return std::make_unique<Variable>(name);
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
        
        std::unique_ptr<ASTNode> Parser::ParseCallExpression()
        {
            std::string callee = GetTokenText(Consume());

            Consume();

            std::vector<std::unique_ptr<ASTNode>> args;

            while(Current().getType() != Lexing::TokenType::RightParen)
            {
                args.push_back(ParseExpression());
                if(Current().getType() == Lexing::TokenType::RightParen)
                    break;
                ExpectToken(Lexing::TokenType::Comma);
                Consume();
            }
            Consume();

            return std::make_unique<CallExpression>(callee, std::move(args));
        }

        std::unique_ptr<ASTNode> Parser::ParseForStatement()
        {
            Consume();

            ExpectToken(Lexing::TokenType::LeftParen);
            Consume();

            std::unique_ptr<ASTNode> init = ParseExpression();

            ExpectToken(Lexing::TokenType::Semicolon);
            Consume();

            std::unique_ptr<ASTNode> cond = ParseExpression();

            ExpectToken(Lexing::TokenType::Semicolon);
            Consume();

            std::unique_ptr<ASTNode> iter = ParseExpression();

            ExpectToken(Lexing::TokenType::RightParen);
            Consume();

            std::shared_ptr<Environment> scope = std::make_shared<Environment>();
            scope->outer = _currentScope;
            _currentScope = scope;

            std::unique_ptr<ASTNode> body = ParseExpression();

            _currentScope = scope->outer;

            return std::make_unique<ForStatement>(std::move(init), std::move(cond), std::move(iter), std::move(body), scope);
        }

        std::unique_ptr<ASTNode> Parser::ParseSubscript()
        {
            std::unique_ptr<ASTNode> operand = ParseVariable();
            Consume();

            std::unique_ptr<ASTNode> index = ParseExpression();

            ExpectToken(Lexing::TokenType::RightSquareBracket);
            Consume();

            return std::make_unique<SubscriptExpression>(std::move(operand), std::move(index));
        }

        std::unique_ptr<ASTNode> Parser::ParseString()
        {
            return std::make_unique<StringLiteral>(GetTokenText(Consume()));
        }
    }
}