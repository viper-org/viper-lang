// Copyright 2023 solar-mist


#include "parser/ast/expression/CallExpression.h"

#include <vipir/IR/Instruction/CallInst.h>

namespace parsing
{
    CallExpression::CallExpression(ASTNodePtr callee)
        : mCallee(std::move(callee))
    {
    }

    vipir::Value* CallExpression::emit(vipir::Builder& builder, vipir::Module& module)
    {
        vipir::Value* callee = mCallee->emit(builder, module);

        return builder.CreateCall(callee, {});
    }
}