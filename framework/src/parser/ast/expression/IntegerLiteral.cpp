// Copyright 2023 solar-mist


#include "parser/ast/expression/IntegerLiteral.h"

#include <vipir/IR/Constant/ConstantInt.h>

namespace parsing
{
    IntegerLiteral::IntegerLiteral(long long value)
        : mValue(value)
    {
    }

    vipir::Value* IntegerLiteral::emit(vipir::Builder& builder, vipir::Module& module)
    {
        return builder.CreateConstantInt(mValue, vipir::Type::GetIntegerType(32));
    }
}