// Copyright 2024 solar-mist

#include "parser/ast/expression/IntegerLiteral.h"

#include <vipir/IR/Constant/ConstantInt.h>

namespace parser
{
    IntegerLiteral::IntegerLiteral(intmax_t value)
        : mValue(value)
    {
    }

    vipir::Value* IntegerLiteral::emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope)
    {
        return builder.CreateConstantInt(mValue);
    }

}