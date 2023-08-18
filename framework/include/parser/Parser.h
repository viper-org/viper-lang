// Copyright 2023 solar-mist

#ifndef VIPER_FRAMEWORK_PARSER_PARSER_H
#define VIPER_FRAMEWORK_PARSER_PARSER_H 1

#include "parser/ast/Node.h"
#include "parser/ast/global/Function.h"
#include "parser/ast/statement/ReturnStatement.h"
#include "parser/ast/statement/VariableDeclaration.h"
#include "parser/ast/expression/IntegerLiteral.h"
#include "parser/ast/expression/Variable.h"
#include "lexer/Token.h"

#include <vector>

namespace parsing
{
    class Parser
    {
    public:
        Parser(std::vector<lexing::Token>& tokens);

        std::vector<ASTNodePtr> parse();

    private:
        std::vector<lexing::Token>& mTokens;
        int mPosition;

        lexing::Token current() const;
        lexing::Token consume();
        lexing::Token peek(int offset) const;

        void expectToken(lexing::TokenType tokenType);

        int getBinaryOperatorPrecedence(lexing::TokenType tokenType);

        ASTNodePtr parseExpression(int precedence = 1);
        ASTNodePtr parsePrimary();

        FunctionPtr parseFunction();

        ReturnStatementPtr parseReturnStatement();
        VariableDeclarationPtr parseVariableDeclaration();

        IntegerLiteralPtr parseIntegerLiteral();
        VariablePtr parseVariable();
    };
}

#endif