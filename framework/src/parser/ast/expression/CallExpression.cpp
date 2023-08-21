// Copyright 2023 solar-mist


#include "parser/ast/expression/CallExpression.h"

#include <vipir/IR/Instruction/CallInst.h>

namespace parsing
{
    CallExpression::CallExpression(ASTNodePtr callee, std::vector<ASTNodePtr> parameters)
        : mCallee(std::move(callee))
        , mParameters(std::move(parameters))
    {
    }

    vipir::Value* CallExpression::emit(vipir::Builder& builder, vipir::Module& module, Environment* scope)
    {
        vipir::Value* callee = mCallee->emit(builder, module, scope);

        std::vector<vipir::Value*> parameters;
        for (auto& parameter : mParameters)
        {
            parameters.push_back(parameter->emit(builder, module, scope));
        }

        return builder.CreateCall(callee, parameters);
    }
}