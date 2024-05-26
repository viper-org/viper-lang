// Copyright 2024 solar-mist

#include "parser/ast/expression/StringLiteral.h"

#include "type/PointerType.h"

#include <vipir/IR/GlobalString.h>

#include <vipir/IR/Instruction/AddrInst.h>

namespace parser
{
    StringLiteral::StringLiteral(std::string value, lexing::Token token)
        : mValue(value)
    {
        mType = PointerType::Create(Type::Get("i8"));
        mPreferredDebugToken = std::move(token);
    }

    void StringLiteral::typeCheck(Scope* scope, diagnostic::Diagnostics& diag)
    {
        if (!mType->isPointerType() || !static_cast<PointerType*>(mType)->getBaseType()->isIntegerType()
         || static_cast<PointerType*>(mType)->getBaseType()->getSize() != 8)
            {
                diag.compilerError(mPreferredDebugToken.getStart(), mPreferredDebugToken.getEnd(), std::format("String literal must have type '{}i8*{}",
                    fmt::bold, fmt::defaults));
            }
    }

    vipir::Value* StringLiteral::emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope, diagnostic::Diagnostics& diag)
    {
        vipir::GlobalString* string = vipir::GlobalString::Create(module, std::move(mValue));

        return builder.CreateAddrOf(string);
    }
}