// Copyright 2024 solar-mist

#include "parser/ast/expression/IntegerLiteral.h"

#include <vipir/IR/Constant/ConstantInt.h>

namespace parser
{
    IntegerLiteral::IntegerLiteral(intmax_t value, Type* type, lexing::Token token)
        : mValue(value)
    {
        mType = type ? type : Type::Get("i32");
        mPreferredDebugToken = std::move(token);
    }

    intmax_t IntegerLiteral::getValue() const
    {
        return mValue;
    }

    void IntegerLiteral::typeCheck(Scope* scope, diagnostic::Diagnostics& diag)
    {
        if (!mType->isIntegerType())
        {
            diag.compilerError(mPreferredDebugToken.getStart(), mPreferredDebugToken.getEnd(), std::format("Integer literal cannot have type '{}{}{}",
                fmt::bold, mType->getName(), fmt::defaults));
        }
    }

    vipir::Value* IntegerLiteral::emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope, diagnostic::Diagnostics& diag)
    {
        return vipir::ConstantInt::Get(module, mValue, mType->getVipirType());
    }
}