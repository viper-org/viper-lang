// Copyright 2023 solar-mist


#include "parser/ast/expression/Variable.h"

#include "environment/Environment.h"

#include <vipir/IR/Instruction/LoadInst.h>
#include <vipir/IR/Function.h>

namespace parsing
{
    Variable::Variable(const std::string& name, Type* type)
        : mName(name)
    {
        mType = type;
    }

    vipir::Value* Variable::emit(vipir::Builder& builder, vipir::Module& module, Environment* scope)
    {
        auto function = functions.find(mName);
        if (function != functions.end())
        {
            return function->second;
        }

        LocalSymbol variable = scope->findVariable(mName);
        return builder.CreateLoad(variable.alloca);
    }
}