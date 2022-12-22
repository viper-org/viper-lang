#include <iostream>
#include <parsing/parser.hh>
#include <lexing/lexer.hh>
#include <diagnostics.hh>
#include <type/types.hh>
#include <charconv>
#include <fstream>
#include <sstream>

namespace Parsing
{
    Parser::Parser(const std::vector<Lexing::Token>& tokens, const std::string& text, llvm::LLVMContext& ctx, const std::vector<std::shared_ptr<VarSymbol>>& symbols)
        :_text(text), _tokens(tokens), _position(0), _ctx(ctx), _currentScope(std::make_shared<Environment>(nullptr)), _currentReturnType(nullptr)
    {
        _currentScope->GetVarSymbols() = symbols;
    }

    std::vector<std::pair<std::unique_ptr<ASTNode>, std::shared_ptr<VarSymbol>>> Parser::ParseLibrary()
    {
        std::vector<std::pair<std::unique_ptr<ASTNode>, std::shared_ptr<VarSymbol>>> declarations;
        while(_position < _tokens.size())
        {
            ExpectToken(Lexing::TokenType::Asperand); // Add struct symbol resolution
            Consume();

            bool isExtension = false;

            std::string mangledName = Consume().GetText();
            if(mangledName == "Main" || mangledName == "_start")
                continue;
            if(mangledName[2] == 'N')
                throw; // Add member function resolution
            
            std::string length;
            int position = 2;
            while(std::isdigit(mangledName[position]))
                length += mangledName[position++];
            
            int nameLength = std::stoi(length);

            std::string name = mangledName.substr(position, nameLength);
            position += nameLength;

            if(mangledName[position] == 'T')
            {
                isExtension = true;
                position++;
            }

            std::string argNum;
            while(std::isdigit(mangledName[position]))
                argNum += mangledName[position++];
            
            int numArgs = std::stoi(argNum);
            if(isExtension)
                llvm::outs() << numArgs << "\n";
            std::vector<std::pair<std::shared_ptr<Type>, std::string>> args;
            for(int i = 0; i < numArgs; i++)
            {
                std::shared_ptr<Type> type;
                switch(mangledName[position++])
                {
                    case 'c':
                        type = types.at("int8");
                        break;
                    case 'w':
                        type = types.at("int16");
                        break;
                    case 'i':
                        type = types.at("int32");
                        break;
                    case 'q':
                        type = types.at("int64");
                        break;
                    case 'b':
                        type = types.at("bool");
                        break;
                }
                while(mangledName[position] == 'P')
                {
                    position++;
                    type = std::make_shared<PointerType>(type);
                }
                args.push_back(std::make_pair(type, ""));
            }
            position++;
            std::shared_ptr<Type> returnType;
            switch(mangledName[position++])
            {
                case 'c':
                    returnType = types.at("int8");
                    break;
                case 'w':
                    returnType = types.at("int16");
                    break;
                case 'i':
                    returnType = types.at("int32");
                    break;
                case 'q':
                    returnType = types.at("int64");
                    break;
                case 'b':
                    returnType = types.at("bool");
                    break;
                case 'V':
                    returnType = types.at("void");
                    break;
            }
            while(mangledName[position] == 'P')
            {
                position++;
                returnType = std::make_shared<PointerType>(returnType);
            }
            declarations.push_back(std::make_pair(
                std::make_unique<ImportStatement>(name, returnType, args, isExtension),
                std::make_shared<VarSymbol>(name, returnType)));
        }
        return declarations;
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
            case Lexing::TokenType::LeftSquareBracket:
            case Lexing::TokenType::LeftParen:
                return 55;
            case Lexing::TokenType::Dot:
                return 50;
            case Lexing::TokenType::As:
                return 45;
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
            
            case Lexing::TokenType::DoubleAmpersand:
                return 20;
            case Lexing::TokenType::DoublePipe:
                return 15;
        
            case Lexing::TokenType::Equals:
            case Lexing::TokenType::PlusEquals:
            case Lexing::TokenType::MinusEquals:
            case Lexing::TokenType::StarEquals:
            case Lexing::TokenType::SlashEquals:
                return 10;
            
            default:
                return 0;
        }
    }

    int Parser::GetUnOpPrecedence(Lexing::TokenType type)
    {
        switch(type)
        {
            case Lexing::TokenType::Bang:
            case Lexing::TokenType::Minus:
            case Lexing::TokenType::Hash:
            case Lexing::TokenType::Asperand:
            case Lexing::TokenType::New:
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

            if(expr->GetNodeType() == ASTNodeType::Function || expr->GetNodeType() == ASTNodeType::ImportStatement || expr->GetNodeType() == ASTNodeType::ClassDefinition)
                result.push_back(std::move(expr));
            else
            {
                _position = savePos;
                ParserError("Expected top-level expression, found " + Current().TypeAsString());
            }
        }
        return result;
    }
    
    std::unique_ptr<ASTNode> Parser::ParseExpression(int precedence)
    {
        std::unique_ptr<ASTNode> lhs;
        int unOpPrecedence = GetUnOpPrecedence(Current().GetType());
        if(unOpPrecedence && unOpPrecedence >= precedence)
        {
            Lexing::Token operatorToken = Consume();
            if(operatorToken.GetType() == Lexing::TokenType::New)
            {
                std::unique_ptr<ASTNode> operand = std::make_unique<Variable>(Consume().GetText(), nullptr);
                lhs = std::make_unique<UnaryExpression>(std::move(operand), operatorToken);
            }
            else
                lhs = std::make_unique<UnaryExpression>(ParseExpression(unOpPrecedence), operatorToken);
        }
        else
            lhs = ParsePrimary();

        while(true)
        {
            int binOpPrecedence = GetBinOpPrecedence(Current().GetType());
            if(binOpPrecedence < precedence)
                break;

            Lexing::Token operatorToken = Consume();
            std::unique_ptr<ASTNode> rhs;
            std::shared_ptr<Type> type;
            if(operatorToken.GetType() == Lexing::TokenType::Dot)
                rhs = std::make_unique<Variable>(Consume().GetText(), nullptr);
            else if(operatorToken.GetType() == Lexing::TokenType::As)
                type = ParseType();
            else if(operatorToken.GetType() == Lexing::TokenType::LeftParen)
                lhs = ParseCallExpression(std::move(lhs));
            else
                rhs = ParseExpression(binOpPrecedence);

            if(operatorToken.GetType() != Lexing::TokenType::LeftParen)
                lhs = std::make_unique<BinaryExpression>(std::move(lhs), operatorToken, std::move(rhs));

            if(operatorToken.GetType() == Lexing::TokenType::As)
                lhs->SetType(type);

            if(operatorToken.GetType() == Lexing::TokenType::LeftSquareBracket)
            {
                ExpectToken(Lexing::TokenType::RightSquareBracket);
                Consume();
            }
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
            case Lexing::TokenType::String:
                return ParseStringLiteral();
            case Lexing::TokenType::Identifier:
                return ParseVariable();
            case Lexing::TokenType::LeftBracket:
                return ParseCompoundExpression();
            case Lexing::TokenType::LeftParen:
                return ParseParenthesizedExpression();
            case Lexing::TokenType::If:
                return ParseIfStatement();
            case Lexing::TokenType::While:
                return ParseWhileStatement();
            case Lexing::TokenType::Import:
                return ParseImportStatement();
            case Lexing::TokenType::Struct:
                return ParseStructDeclaration();
            case Lexing::TokenType::Class:
                return ParseClassDefinition();
            default:
                ParserError("Expected primary expression, found '" + Current().GetText() + "'");
        }
    }

    std::shared_ptr<Type> Parser::ParseType()
    {
        //ExpectToken(Lexing::TokenType::Type); Add struct definition table
        std::shared_ptr<Type> type = types.at(Consume().GetText());
        while(Current().GetType() == Lexing::TokenType::Star || Current().GetType() == Lexing::TokenType::LeftSquareBracket)
        {
            if(Current().GetType() == Lexing::TokenType::Star)
            {
                Consume();
                type = std::make_shared<PointerType>(type);
            }
            else
            {
                Consume();
                int length = std::stoi(Consume().GetText());
                type = std::make_shared<ArrayType>(length, type);

                ExpectToken(Lexing::TokenType::RightSquareBracket);
                Consume();
            }
        }
        return type;
    }

    std::unique_ptr<ASTNode> Parser::ParseVariableDeclaration()
    {
        Consume();

        std::shared_ptr<Type> type = ParseType();

        ExpectToken(Lexing::TokenType::Identifier);
        std::string name = Consume().GetText();
        
        _currentScope->GetVarSymbols().push_back(std::make_shared<VarSymbol>(name, type));

        std::vector<std::pair<std::shared_ptr<Type>, std::string>> args;
        bool isExtension = false;
        std::shared_ptr<Environment> scope = nullptr;
        if(Current().GetType() == Lexing::TokenType::LeftParen)
        {
            Consume();
            scope = std::make_shared<Environment>(_currentScope);
            _currentScope = scope;
            while(Current().GetType() != Lexing::TokenType::RightParen)
            {
                std::shared_ptr<Type> type;
                std::string argName;
                if(Current().GetText() == "this")
                {
                    isExtension = true;
                    Consume();
                    type = ParseType();
                    argName = "this";
                }
                else
                {
                    type = ParseType();

                    ExpectToken(Lexing::TokenType::Identifier);
                    argName = Consume().GetText();
                }
                
                args.push_back(std::make_pair(type, argName));
                _currentScope->GetVarSymbols().push_back(std::make_shared<VarSymbol>(argName, type));
                if(Current().GetType() == Lexing::TokenType::RightParen)
                    break;

                ExpectToken(Lexing::TokenType::Comma);
                Consume();
            }
            Consume();
            _currentReturnType = type;
        }

        if(Current().GetType() != Lexing::TokenType::Equals)
        {
            if(scope)
            {
                _currentScope = _currentScope->GetOuter();
                return std::make_unique<Function>(name, nullptr, scope, type, args, isExtension);
            }

            return std::make_unique<VariableDeclaration>(name, nullptr, type);
        }

        Consume();
        
        std::unique_ptr<ASTNode> initVal = ParseExpression();

        if(scope)
        {
            _currentScope = _currentScope->GetOuter();
            return std::make_unique<Function>(name, std::move(initVal), scope, type, args, isExtension);
        }
    
        return std::make_unique<VariableDeclaration>(name, std::move(initVal), type);
    }

    std::unique_ptr<ASTNode> Parser::ParseVariable()
    {
        std::string name = Consume().GetText();

        std::shared_ptr<VarSymbol> symbol = _currentScope->FindVarSymbol(name);

        return std::make_unique<Variable>(name, symbol->GetType());
    }

    std::unique_ptr<ASTNode> Parser::ParseImportStatement()
    {
        Consume();

        std::shared_ptr<Type> type = ParseType();

        std::string name = Consume().GetText();

        _currentScope->GetVarSymbols().push_back(std::make_shared<VarSymbol>(name, type));

        std::vector<std::pair<std::shared_ptr<Type>, std::string>> args;
        bool isExtension = false;
        ExpectToken(Lexing::TokenType::LeftParen);
        Consume();
        while(Current().GetType() != Lexing::TokenType::RightParen)
        {
            std::shared_ptr<Type> type;
            std::string argName;
            if(Current().GetText() == "this")
            {
                isExtension = true;
                Consume();
                type = ParseType();
                argName = "this";
            }
            else
            {
                type = ParseType();
                ExpectToken(Lexing::TokenType::Identifier);
                argName = Consume().GetText();
            }
            
            args.push_back(std::make_pair(type, argName));
            if(Current().GetType() == Lexing::TokenType::RightParen)
                break;

            ExpectToken(Lexing::TokenType::Comma);
            Consume();
        }
        Consume();

        return std::make_unique<ImportStatement>(name, type, std::move(args), isExtension);
    }

    std::unique_ptr<ASTNode> Parser::ParseClassDefinition()
    {
        Consume();
        ExpectToken(Lexing::TokenType::Identifier);
        std::string name = Consume().GetText();
        
        std::shared_ptr<StructType> structType = StructType::Create(name, std::vector<std::pair<std::shared_ptr<Type>, std::string>>(), _ctx);
        types[name] = structType;

        ExpectToken(Lexing::TokenType::LeftBracket);
        Consume();
        std::vector<std::pair<std::shared_ptr<Type>, std::string>> structTypeFields;
        std::vector<ClassField> classFields;
        std::vector<ClassMethod> classMethods;
        while(Current().GetType() != Lexing::TokenType::RightBracket)
        {
            if(Current().GetText() == name)
            {
                Consume();
                ExpectToken(Lexing::TokenType::LeftParen);
                std::shared_ptr<Environment> scope = std::make_shared<Environment>(_currentScope);
                _currentScope = scope;
                std::vector<std::pair<std::shared_ptr<Type>, std::string>> params;
                Consume();
                while(Current().GetType() != Lexing::TokenType::RightParen)
                {
                    std::shared_ptr<Type> type;
                    std::string argName;
                    type = ParseType();
                    ExpectToken(Lexing::TokenType::Identifier);
                    argName = Consume().GetText();
                    
                    params.push_back(std::make_pair(type, argName));
                    scope->GetVarSymbols().push_back(std::make_shared<VarSymbol>(argName, type));
                    if(Current().GetType() == Lexing::TokenType::RightParen)
                        break;

                    ExpectToken(Lexing::TokenType::Comma);
                    Consume();
                }
                Consume();

                ExpectToken(Lexing::TokenType::Equals);
                Consume();

                scope->GetVarSymbols().push_back(std::make_shared<VarSymbol>("this", structType));

                std::unique_ptr<ASTNode> body = ParseExpression();

                _currentScope = scope->GetOuter();

                ExpectToken(Lexing::TokenType::Semicolon);
                Consume();

                classMethods.push_back({
                    AccessLevel::Public, nullptr, name,
                    params, scope, std::move(body)
                });
            }
            else
            {
                std::shared_ptr<Type> fieldType = ParseType();
                std::string name = Consume().GetText();
                if(Current().GetType() == Lexing::TokenType::LeftParen)
                {
                    std::shared_ptr<Environment> scope = std::make_shared<Environment>(_currentScope);
                    _currentScope = scope;
                    _currentReturnType = fieldType;
                    std::vector<std::pair<std::shared_ptr<Type>, std::string>> params;
                    Consume();
                    while(Current().GetType() != Lexing::TokenType::RightParen)
                    {
                        std::shared_ptr<Type> type = ParseType();

                        ExpectToken(Lexing::TokenType::Identifier);
                        std::string argName = Consume().GetText();
                        
                        params.push_back(std::make_pair(type, argName));
                        scope->GetVarSymbols().push_back(std::make_shared<VarSymbol>(argName, type));
                        if(Current().GetType() == Lexing::TokenType::RightParen)
                            break;

                        ExpectToken(Lexing::TokenType::Comma);
                        Consume();
                    }
                    Consume();

                    ExpectToken(Lexing::TokenType::Equals);
                    Consume();

                    scope->GetVarSymbols().push_back(std::make_shared<VarSymbol>("this", std::make_shared<PointerType>(structType)));

                    std::unique_ptr<ASTNode> body = ParseExpression();

                    _currentScope = scope->GetOuter();

                    ExpectToken(Lexing::TokenType::Semicolon);
                    Consume();

                    classMethods.push_back({
                        AccessLevel::Public, fieldType, name,
                        params, scope, std::move(body)
                    });
                }
                else
                {
                    ExpectToken(Lexing::TokenType::Semicolon);
                    Consume();
                    structTypeFields.push_back(std::make_pair(fieldType, name));
                    classFields.push_back({
                        AccessLevel::Public, fieldType, name
                    });
                }
            }
        }
        Consume();
        
        static_cast<StructType*>(structType.get())->SetBody(structTypeFields);

        _tokens.insert(_tokens.begin() + _position, Lexing::Token(Lexing::TokenType::Semicolon, "", 0, 0, 0, 0));

        return std::make_unique<ClassDefinition>(name, classFields, std::move(classMethods));
    }

    std::unique_ptr<ASTNode> Parser::ParseStructDeclaration()
    {
        Consume();
        ExpectToken(Lexing::TokenType::Identifier);
        std::string name = Consume().GetText();

        ExpectToken(Lexing::TokenType::LeftBracket);
        Consume();
        std::vector<std::pair<std::shared_ptr<Type>, std::string>> fields;
        while(Current().GetType() != Lexing::TokenType::RightBracket)
        {
            std::shared_ptr<Type> type = ParseType();
            std::string name = Consume().GetText();
            ExpectToken(Lexing::TokenType::Semicolon);
            Consume();
            fields.push_back(std::make_pair(type, name));
        }
        Consume();

        types[name] = StructType::Create(name, std::move(fields), _ctx);

        return ParseExpression();
    }

    std::unique_ptr<ASTNode> Parser::ParseCallExpression(std::unique_ptr<ASTNode> callee)
    {
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

        return std::make_unique<CallExpr>(std::move(callee), std::move(args));
    }

    std::unique_ptr<ASTNode> Parser::ParseIntegerLiteral()
    {
        long long value = std::stoll(Consume().GetText());

        return std::make_unique<IntegerLiteral>(value);
    }

    std::unique_ptr<ASTNode> Parser::ParseStringLiteral()
    {
        std::string value = Consume().GetText();

        return std::make_unique<StringLiteral>(value);
    }

    std::unique_ptr<ASTNode> Parser::ParseReturnStatement()
    {
        Consume();

        if(Current().GetType() == Lexing::TokenType::Semicolon)
            return std::make_unique<ReturnStatement>(nullptr, _currentReturnType);

        return std::make_unique<ReturnStatement>(ParseExpression(), _currentReturnType);
    }

    std::unique_ptr<ASTNode> Parser::ParseCompoundExpression()
    {
        Consume();

        std::shared_ptr<Environment> scope = std::make_shared<Environment>(_currentScope);
        _currentScope = scope;

        std::vector<std::unique_ptr<ASTNode>> exprs;

        while(Current().GetType() != Lexing::TokenType::RightBracket)
        {
            exprs.push_back(ParseExpression());
            ExpectToken(Lexing::TokenType::Semicolon);
            Consume();
        }
        Consume();

        _tokens.insert(_tokens.begin() + _position, Lexing::Token(Lexing::TokenType::Semicolon, "", 0, 0, 0, 0));

        _currentScope = scope->GetOuter();

        return std::make_unique<CompoundStatement>(exprs, scope);
    }

    std::unique_ptr<ASTNode> Parser::ParseParenthesizedExpression()
    {
        Consume();

        std::unique_ptr<ASTNode> expr = ParseExpression();

        ExpectToken(Lexing::TokenType::RightParen);
        Consume();

        return expr;
    }

    std::unique_ptr<ASTNode> Parser::ParseIfStatement()
    {
        Consume();

        ExpectToken(Lexing::TokenType::LeftParen);
        Consume();

        std::unique_ptr<ASTNode> cond = ParseExpression();

        ExpectToken(Lexing::TokenType::RightParen);
        Consume();

        std::unique_ptr<ASTNode> body = ParseExpression();

        if(Peek(1).GetType() == Lexing::TokenType::Else)
        {
            ExpectToken(Lexing::TokenType::Semicolon);
            Consume();
            Consume();
            return std::make_unique<IfStatement>(std::move(cond), std::move(body), ParseExpression());
        }

        return std::make_unique<IfStatement>(std::move(cond), std::move(body), nullptr);
    }

    std::unique_ptr<ASTNode> Parser::ParseWhileStatement()
    {
        Consume();

        ExpectToken(Lexing::TokenType::LeftParen);
        Consume();

        std::unique_ptr<ASTNode> cond = ParseExpression();

        ExpectToken(Lexing::TokenType::RightParen);
        Consume();

        std::unique_ptr<ASTNode> body = ParseExpression();

        return std::make_unique<WhileStatement>(std::move(cond), std::move(body));
    }
}