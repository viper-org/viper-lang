// Copyright 2024 solar-mist

#ifndef VIPER_FRAMEWORK_PARSER_AST_STATEMENT_COMPOUND_STATEMENT_H
#define VIPER_FRAMEWORK_PARSER_AST_STATEMENT_COMPOUND_STATEMENT_H 1

#include "parser/ast/Node.h"

namespace parser
{
    class CompoundStatement : public ASTNode
    {
    public:
        CompoundStatement(std::vector<ASTNodePtr>&& body, Scope* scope);

        void typeCheck(Scope* scope, diagnostic::Diagnostics& diag) override;
        vipir::Value* emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope, diagnostic::Diagnostics& diag) override;

    private:
        std::vector<ASTNodePtr> mBody;
        ScopePtr mScope;
    };
    using CompoundStatementPtr = std::unique_ptr<CompoundStatement>;
}

#endif // VIPER_FRAMEWORK_PARSER_AST_STATEMENT_COMPOUND_STATEMENT_H