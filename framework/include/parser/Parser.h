// Copyright 2024 solar-mist

#ifndef VIPER_FRAMEWORK_PARSER_PARSER_H
#define VIPER_FRAMEWORK_PARSER_PARSER_H 1

#include "parser/ast/ASTNode.h"

#include "parser/ast/global/Function.h"

#include "parser/ast/statement/ReturnStatement.h"

#include "parser/ast/expression/IntegerLiteral.h"

#include "lexer/Token.h"

#include "diagnostic/Diagnostic.h"

#include "symbol/Scope.h"

namespace parser
{
    class Parser
    {
    public:
        Parser(std::vector<lexer::Token>& tokens, diagnostic::Diagnostics& diag, Scope* globalScope);

        std::vector<ASTNodePtr> parse();

    private:
        std::vector<lexer::Token>& mTokens;
        unsigned int mPosition;

        diagnostic::Diagnostics& mDiag;

        Scope* mActiveScope;


        lexer::Token current() const;
        lexer::Token consume();
        lexer::Token peek(int offset) const;

        void expectToken(lexer::TokenType tokenType);

        ASTNodePtr parseGlobal();
        ASTNodePtr parseExpression();
        ASTNodePtr parsePrimary();

        FunctionPtr parseFunction();

        ReturnStatementPtr parseReturnStatement();

        IntegerLiteralPtr parseIntegerLiteral();
    };
}

#endif // VIPER_FRAMEWORK_PARSER_PARSER_H