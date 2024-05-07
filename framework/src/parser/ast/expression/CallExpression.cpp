// Copyright 2024 solar-mist


#include "parser/ast/expression/CallExpression.h"

#include "parser/ast/global/Function.h"

#include <vipir/IR/Instruction/CallInst.h>

namespace parser
{
    CallExpression::CallExpression(ASTNodePtr function, std::vector<ASTNodePtr> parameters)
        : mFunction(std::move(function))
        , mParameters(std::move(parameters))
    {
        mType = mFunction->getType();
    }

    vipir::Value* CallExpression::emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope)
    {
        vipir::Function* function = static_cast<vipir::Function*>(mFunction->emit(builder, module, scope));

        std::vector<vipir::Value*> parameters;
        for (auto& parameter : mParameters)
        {
            parameters.push_back(parameter->emit(builder, module, scope));
        }

        return builder.CreateCall(function, std::move(parameters));
    }
}