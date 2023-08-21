// Copyright 2023 solar-mist

#ifndef VIPER_FRAMEWORK_PARSER_PARSER_H
#define VIPER_FRAMEWORK_PARSER_PARSER_H 1

#include "parser/ast/Node.h"
#include "parser/ast/global/Function.h"
#include "parser/ast/global/ExternFunction.h"
#include "parser/ast/statement/ReturnStatement.h"
#include "parser/ast/statement/VariableDeclaration.h"
#include "parser/ast/statement/IfStatement.h"
#include "parser/ast/statement/CompoundStatement.h"
#include "parser/ast/expression/IntegerLiteral.h"
#include "parser/ast/expression/Variable.h"
#include "parser/ast/expression/CallExpression.h"
#include "lexer/Token.h"

#include <vector>

namespace parsing
{
    class Parser
    {
    public:
        Parser(std::vector<lexing::Token>& tokens, Environment* scope);

        std::vector<ASTNodePtr> parse();

    private:
        std::vector<lexing::Token>& mTokens;
        int mPosition;
        Environment* mScope;

        lexing::Token current() const;
        lexing::Token consume();
        lexing::Token peek(int offset) const;

        void expectToken(lexing::TokenType tokenType);

        int getBinaryOperatorPrecedence(lexing::TokenType tokenType);

        ASTNodePtr parseGlobal();
        ASTNodePtr parseExpression(int precedence = 1);
        ASTNodePtr parsePrimary();
        Type* parseType();

        FunctionPtr parseFunction();
        ExternFunctionPtr parseExternFunction();

        ReturnStatementPtr parseReturnStatement();
        VariableDeclarationPtr parseVariableDeclaration();
        IfStatementPtr parseIfStatement();
        CompoundStatementPtr parseCompoundStatement();

        IntegerLiteralPtr parseIntegerLiteral();
        VariablePtr parseVariable();
        CallExpressionPtr parseCallExpression(ASTNodePtr callee);
    };
}

#endif