// Copyright 2024 solar-mist

#include "parser/ast/expression/StringLiteral.h"

#include "type/PointerType.h"

#include <vipir/IR/GlobalString.h>

#include <vipir/IR/Instruction/AddrInst.h>

namespace parser
{
    StringLiteral::StringLiteral(std::string value)
        : mValue(value)
    {
        mType = PointerType::Create(Type::Get("i8"));
    }

    vipir::Value* StringLiteral::emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope)
    {
        vipir::GlobalString* string = vipir::GlobalString::Create(module, std::move(mValue));

        return builder.CreateAddrOf(string);
    }
}