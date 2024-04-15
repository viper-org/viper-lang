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

    vipir::Value* CompoundStatement::emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope)
    {
        scope = mScope.get();

        for (ASTNodePtr& node : mBody)
        {
            node->emit(builder, module, scope);
        }

        return nullptr;
    }

}