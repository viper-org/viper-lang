// Copyright 2023 solar-mist


#include "parser/ast/expression/Variable.h"

#include "environment/Environment.h"

#include <vipir/IR/Instruction/LoadInst.h>

namespace parsing
{
    Variable::Variable(const std::string& name)
        : mName(name)
    {
    }

    vipir::Value* Variable::emit(vipir::Builder& builder, vipir::Module& module)
    {
        return builder.CreateLoad(variables.at(mName));
    }
}