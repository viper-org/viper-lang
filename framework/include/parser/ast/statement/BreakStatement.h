// Copyright 2024 solar-mist

#ifndef VIPER_FRAMEWORK_PARSER_AST_STATEMENT_BREAK_STATEMENT_H
#define VIPER_FRAMEWORK_PARSER_AST_STATEMENT_BREAK_STATEMENT_H 1

#include "parser/ast/Node.h"

#include "lexer/Token.h"

namespace parser
{
    class BreakStatement : public ASTNode
    {
    public:
        BreakStatement(lexing::Token token);

        void typeCheck(Scope* scope, diagnostic::Diagnostics& diag) override;
        vipir::Value* emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope, diagnostic::Diagnostics& diag) override;

    private:
        lexing::Token mToken;
    };
    using BreakStatementPtr = std::unique_ptr<BreakStatement>;
}

#endif // VIPER_FRAMEWORK_PARSER_AST_STATEMENT_BREAK_STATEMENT_H