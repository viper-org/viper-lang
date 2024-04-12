// Copyright 2024 solar-mist

#include "parser/ast/global/Function.h"
#include "vipir/IR/BasicBlock.h"

#include <vipir/IR/Function.h>

namespace parser
{
    Function::Function(std::string_view name, std::vector<ASTNodePtr>&& body, Scope* scope)
        : mName(name)
        , mBody(std::move(body))
        , mScope(scope)
    {
    }

    vipir::Value* Function::emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope)
    {
        scope = mScope.get();

        vipir::Function* func = vipir::Function::Create(module, mName);

        vipir::BasicBlock* entryBasicBlock = vipir::BasicBlock::Create("", func);
        builder.setInsertPoint(entryBasicBlock);

        for (auto& node : mBody)
        {
            node->emit(builder, module, scope);
        }

        return func;
    }

}