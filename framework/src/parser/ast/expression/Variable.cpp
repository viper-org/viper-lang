// Copyright 2023 solar-mist


#include "parser/ast/expression/Variable.h"

#include "environment/Environment.h"

#include <vipir/IR/Instruction/LoadInst.h>
#include <vipir/IR/Function.h>

namespace parsing
{
    Variable::Variable(const std::string& name)
        : mName(name)
    {
    }

    vipir::Value* Variable::emit(vipir::Builder& builder, vipir::Module& module, Environment* scope)
    {
        auto function = functions.find(mName);
        if (function != functions.end())
        {
            return function->second;
        }
        return builder.CreateLoad(scope->findVariable(mName));
    }
}