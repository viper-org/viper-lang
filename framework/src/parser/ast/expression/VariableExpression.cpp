// Copyright 2024 solar-mist

#include "parser/ast/expression/VariableExpression.h"

#include <vipir/IR/Instruction/AllocaInst.h>
#include <vipir/IR/Instruction/LoadInst.h>

namespace parser
{
    VariableExpression::VariableExpression(std::string&& name)
        : mName(std::move(name))
    {
    }

    vipir::Value* VariableExpression::emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope)
    {
        vipir::AllocaInst* alloca = scope->findVariable(mName).alloca;

        if (alloca)
        {
            return builder.CreateLoad(alloca);
        }
        else
        {
            return nullptr; // TODO: Error
        }
    }
}