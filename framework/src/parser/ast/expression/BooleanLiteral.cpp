// Copyright 2024 solar-mist

#include "parser/ast/expression/BooleanLiteral.h"

#include <vipir/IR/Constant/ConstantBool.h>

namespace parser
{
    BooleanLiteral::BooleanLiteral(bool value)
        : mValue(value)
    {
        mType = Type::Get("bool");
    }

    vipir::Value* BooleanLiteral::emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope)
    {
        return builder.CreateConstantBool(mValue);
    }
}