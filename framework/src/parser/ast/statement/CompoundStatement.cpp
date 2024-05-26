// Copyright 2024 solar-mist

#include "parser/ast/statement/CompoundStatement.h"

#include <vipir/IR/Instruction/RetInst.h>

#include <vipir/IR/BasicBlock.h>

namespace parser
{
    CompoundStatement::CompoundStatement(std::vector<ASTNodePtr>&& body, Scope* scope)
        : mBody(std::move(body))
        , mScope(scope)
    {
    }

    void CompoundStatement::typeCheck(Scope* scope, diagnostic::Diagnostics& diag)
    {
        for (auto& node : mBody)
        {
            node->typeCheck(scope, diag);
        }
    }

    vipir::Value* CompoundStatement::emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope, diagnostic::Diagnostics& diag)
    {
        scope = mScope.get();

        for (ASTNodePtr& node : mBody)
        {
            node->emit(builder, module, scope, diag);
        }

        return nullptr;
    }

}