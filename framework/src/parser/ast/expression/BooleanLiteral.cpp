// Copyright 2024 solar-mist

#include "parser/ast/expression/BooleanLiteral.h"

#include <vipir/IR/Constant/ConstantBool.h>

namespace parser
{
    BooleanLiteral::BooleanLiteral(bool value, lexing::Token token)
        : mValue(value)
    {
        mType = Type::Get("bool");
        mPreferredDebugToken = std::move(token);
    }

    bool BooleanLiteral::getValue() const
    {
        return mValue;
    }

    void BooleanLiteral::typeCheck(Scope* scope, diagnostic::Diagnostics& diag)
    {
        // something has seriously gone wrong if this is true
        if (!mType->isBooleanType())
        {
            diag.compilerError(mPreferredDebugToken.getStart(), mPreferredDebugToken.getEnd(), std::format("Boolean literal cannot have type '{}{}{}'",
                fmt::bold, mType->getName(), fmt::defaults));
        }
    }

    vipir::Value* BooleanLiteral::emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope, diagnostic::Diagnostics& diag)
    {
        return builder.CreateConstantBool(mValue);
    }
}