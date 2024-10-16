// Copyright 2024 solar-mist

#include "parser/ast/expression/StringLiteral.h"

#include "type/PointerType.h"

#include <vipir/IR/GlobalString.h>

#include <vipir/IR/Instruction/AddrInst.h>

namespace parser
{
    StringLiteral::StringLiteral(Scope* scope, std::string value, lexer::Token token)
        : ASTNode(scope, PointerType::Get(Type::Get("i8")), token)
        , mValue(std::move(value))
    {
    }

    vipir::Value* StringLiteral::codegen(vipir::IRBuilder& builder, vipir::Module& module, diagnostic::Diagnostics& diag)
    {
        vipir::GlobalString* string = vipir::GlobalString::Create(module, std::move(mValue));

        return builder.CreateAddrOf(string);
    }
    
    void StringLiteral::typeCheck(diagnostic::Diagnostics&, bool&)
    {
    }

    bool StringLiteral::triviallyImplicitCast(diagnostic::Diagnostics&, Type*)
    {
        return false;
    }
}