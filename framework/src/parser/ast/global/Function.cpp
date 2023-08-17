// Copyright 2023 solar-mist


#include "parser/ast/global/Function.h"

#include <vipir/IR/Function.h>
#include <vipir/IR/BasicBlock.h>

namespace parsing
{
    Function::Function(const std::string& name, std::vector<ASTNodePtr> body)
        : mName(name)
        , mBody(std::move(body))
    {
    }

    std::string_view Function::getName() const
    {
        return mName;
    }

    const std::vector<ASTNodePtr>& Function::getBody() const
    {
        return mBody;
    }

    vipir::Value* Function::emit(vipir::Builder& builder, vipir::Module& module)
    {
        vipir::Function* function = vipir::Function::Create(module, mName);

        vipir::BasicBlock* entryBB = vipir::BasicBlock::Create(mName, function);
        builder.setInsertPoint(entryBB);

        return function;
    }
}