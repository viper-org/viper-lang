// Copyright 2024 solar-mist


#include "parser/ast/expression/NullptrLiteral.h"
#include "type/PointerType.h"

#include <vipir/IR/Constant/ConstantNullPtr.h>

namespace parser
{
    NullptrLiteral::NullptrLiteral(Type* type, lexing::Token token)
    {
        mType = type ? type : PointerType::Create(Type::Get("i8"));
        mPreferredDebugToken = std::move(token);
    }

    void NullptrLiteral::typeCheck(Scope* scope, diagnostic::Diagnostics& diag)
    {
        if (!mType->isPointerType())
        {
            diag.compilerError(mPreferredDebugToken.getStart(), mPreferredDebugToken.getEnd(), std::format("Null pointer literal cannot have type '{}{}{}'",
                fmt::bold, mType->getName(), fmt::defaults));
        }
    }

    vipir::Value* NullptrLiteral::emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope, diagnostic::Diagnostics& diag)
    {
        return vipir::ConstantNullPtr::Get(module, mType->getVipirType());
    }
}