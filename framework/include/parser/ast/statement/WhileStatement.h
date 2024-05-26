// Copyright 2024 solar-mist

#ifndef VIPER_FRAMEWORK_PARSER_AST_STATEMENT_WHILE_STATEMENT_H
#define VIPER_FRAMEWORK_PARSER_AST_STATEMENT_WHILE_STATEMENT_H 1

#include "parser/ast/Node.h"

namespace parser
{
    class WhileStatement : public ASTNode
    {
    public:
        WhileStatement(ASTNodePtr&& condition, ASTNodePtr&& body, Scope* scope);

        void typeCheck(Scope* scope, diagnostic::Diagnostics& diag) override;
        vipir::Value* emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope, diagnostic::Diagnostics& diag) override;

    private:
        ASTNodePtr mCondition;
        ASTNodePtr mBody;
        ScopePtr mScope;
    };
    using WhileStatementPtr = std::unique_ptr<WhileStatement>;
}

#endif // VIPER_FRAMEWORK_PARSER_AST_STATEMENT_WHILE_STATEMENT_H