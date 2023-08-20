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

    vipir::Value* CallExpression::emit(vipir::Builder& builder, vipir::Module& module)
    {
        vipir::Value* callee = mCallee->emit(builder, module);

        std::vector<vipir::Value*> parameters;
        for (auto& parameter : mParameters)
        {
            parameters.push_back(parameter->emit(builder, module));
        }

        return builder.CreateCall(callee, parameters);
    }
}