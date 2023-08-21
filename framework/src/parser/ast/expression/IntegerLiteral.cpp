// Copyright 2023 solar-mist


#include "parser/ast/expression/IntegerLiteral.h"

#include <vipir/IR/Constant/ConstantInt.h>

namespace parsing
{
    IntegerLiteral::IntegerLiteral(long long value)
        : mValue(value)
    {
    }

    long long IntegerLiteral::getValue() const
    {
        return mValue;
    }

    vipir::Value* IntegerLiteral::emit(vipir::Builder& builder, vipir::Module& module, Environment* scope)
    {
        return builder.CreateConstantInt(mValue, vipir::Type::GetIntegerType(32));
    }
}