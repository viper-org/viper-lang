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

    vipir::Value* Variable::emit(vipir::Builder& builder, vipir::Module& module)
    {
        //return builder.CreateLoad(variables.at(mName));
        auto alloca = variables.find(mName);
        if (alloca != variables.end())
        {
            return builder.CreateLoad(alloca->second);
        }
        return functions.at(mName);
    }
}