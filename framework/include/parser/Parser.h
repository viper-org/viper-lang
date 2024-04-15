// Copyright 2023 solar-mist

#ifndef VIPER_FRAMEWORK_PARSER_PARSER_H
#define VIPER_FRAMEWORK_PARSER_PARSER_H 1

#include "parser/ast/Node.h"
#include "parser/ast/global/Function.h"
#include "parser/ast/statement/ReturnStatement.h"
#include "parser/ast/statement/VariableDeclaration.h"
#include "parser/ast/statement/IfStatement.h"
#include "parser/ast/statement/WhileStatement.h"
#include "parser/ast/statement/CompoundStatement.h"
#include "parser/ast/expression/IntegerLiteral.h"
#include "parser/ast/expression/VariableExpression.h"

#include "lexer/Token.h"

#include <vector>

namespace parser
{
    struct Symbol
    {
        std::string name;
        Type* type;
    };

    class Parser
    {
    public:
        Parser(std::vector<lexing::Token>& tokens);

        std::vector<ASTNodePtr> parse();

    private:
        std::vector<lexing::Token>& mTokens;
        int mPosition;

        Scope* mScope;
        std::vector<Symbol> mSymbols;

        lexing::Token current() const;
        lexing::Token consume();
        lexing::Token peek(int offset) const;

        void expectToken(lexing::TokenType tokenType);

        int getBinaryOperatorPrecedence(lexing::TokenType tokenType);

        Type* parseType();

        ASTNodePtr parseGlobal();
        ASTNodePtr parseExpression(Type* preferredType = nullptr, int precedence = 1);
        ASTNodePtr parsePrimary(Type* preferredType = nullptr);

        FunctionPtr parseFunction();

        CompoundStatementPtr parseCompoundStatement();
        ReturnStatementPtr parseReturnStatement();
        VariableDeclarationPtr parseVariableDeclaration();
        IfStatementPtr parseIfStatement();
        WhileStatementPtr parseWhileStatement();

        IntegerLiteralPtr parseIntegerLiteral(Type* preferredType = nullptr);
        VariableExpressionPtr parseVariableExpression(Type* preferredType = nullptr);
    };
}

#endif