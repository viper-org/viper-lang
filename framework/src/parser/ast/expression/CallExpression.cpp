// Copyright 2024 solar-mist


#include "parser/ast/expression/CallExpression.h"

#include <vipir/IR/Instruction/CallInst.h>

namespace parser
{
    CallExpression::CallExpression(ASTNodePtr function)
        : mFunction(std::move(function))
    {
    }

    vipir::Value* CallExpression::emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope)
    {
        vipir::Function* function = static_cast<vipir::Function*>(mFunction->emit(builder, module, scope));

        return builder.CreateCall(function);
    }
}