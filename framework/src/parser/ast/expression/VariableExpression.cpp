// Copyright 2024 solar-mist

#include "parser/ast/expression/VariableExpression.h"

#include <vipir/IR/Instruction/AllocaInst.h>
#include <vipir/IR/Instruction/LoadInst.h>

namespace parser
{
    VariableExpression::VariableExpression(std::string&& name, Type* type)
        : mName(std::move(name))
    {
        mType = type;
    }

    vipir::Value* VariableExpression::emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope)
    {
        LocalSymbol* local = scope->findVariable(mName);

        if (local)
        {
            return builder.CreateLoad(local->alloca);
        }
        else if (GlobalFunctions.find(mName) != GlobalFunctions.end())
        {
            return GlobalFunctions.at(mName).function;
        }
        else if (GlobalVariables.find(mName) != GlobalVariables.end())
        {
            return builder.CreateLoad(GlobalVariables.at(mName).global);
        }
        else
        {
            return nullptr; // TODO: Error
        }
    }
}