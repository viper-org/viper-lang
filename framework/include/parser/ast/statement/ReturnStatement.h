// Copyright 2024 solar-mist

#ifndef VIPER_FRAMEWORK_PARSER_AST_STATEMENT_RETURN_STATEMENT_H
#define VIPER_FRAMEWORK_PARSER_AST_STATEMENT_RETURN_STATEMENT_H 1

#include "parser/ast/Node.h"

namespace parser
{
    class ReturnStatement : public ASTNode
    {
    public:
        ReturnStatement(ASTNodePtr&& returnValue);

        void typeCheck(Scope* scope, diagnostic::Diagnostics& diag) override;
        vipir::Value* emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope, diagnostic::Diagnostics& diag) override;

    private:
        ASTNodePtr mReturnValue;
    };
    using ReturnStatementPtr = std::unique_ptr<ReturnStatement>;
}

#endif // VIPER_FRAMEWORK_PARSER_AST_STATEMENT_RETURN_STATEMENT_H