// Copyright 2024 solar-mist

#include "parser/ast/expression/IntegerLiteral.h"

#include <vipir/IR/Constant/ConstantInt.h>

namespace parser
{
    IntegerLiteral::IntegerLiteral(intmax_t value, Type* type)
        : mValue(value)
    {
        mType = type ? type : Type::Get("i32");
    }

    intmax_t IntegerLiteral::getValue() const
    {
        return mValue;
    }

    vipir::Value* IntegerLiteral::emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope, diagnostic::Diagnostics& diag)
    {
        return vipir::ConstantInt::Get(module, mValue, mType->getVipirType());
    }
}