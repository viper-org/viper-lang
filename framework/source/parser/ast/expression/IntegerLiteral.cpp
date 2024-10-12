// Copyright 2024 solar-mist

#include "parser/ast/expression/IntegerLiteral.h"

#include <vipir/IR/Constant/ConstantInt.h>

namespace parser
{
    IntegerLiteral::IntegerLiteral(Scope* scope, std::uintmax_t value, lexer::Token token)
        : ASTNode(scope, Type::Get("i32"), token)
        , mValue(value)
    {
    }

    vipir::Value* IntegerLiteral::codegen(vipir::IRBuilder& builder, vipir::Module& module, diagnostic::Diagnostics& diag)
    {
        return vipir::ConstantInt::Get(module, mValue, vipir::Type::GetIntegerType(32));
    }
    
    void IntegerLiteral::typeCheck(diagnostic::Diagnostics&, bool&)
    {
    }
}