// Copyright 2024 solar-mist

#include "parser/ast/expression/CastExpression.h"

#include <vipir/IR/Instruction/TruncInst.h>
#include <vipir/IR/Instruction/SExtInst.h>

#include <cmath>

namespace parser
{
    CastExpression::CastExpression(Scope* scope, ASTNodePtr value, Type* destType)
        : ASTNode(scope, destType, lexer::Token("", lexer::TokenType::Error, {}, {}))
        , mValue(std::move(value))
    {
    }

    vipir::Value* CastExpression::codegen(vipir::IRBuilder& builder, vipir::Module& module, diagnostic::Diagnostics& diag)
    {
        auto value = mValue->codegen(builder, module, diag);
        if (mType->isIntegerType() && mValue->getType()->isIntegerType())
        {
            if (mType->getSize() < mValue->getType()->getSize())
            {
                return builder.CreateTrunc(value, mType->getVipirType());
            }
            else
            {
                return builder.CreateSExt(value, mType->getVipirType());
            }
        }
        return nullptr; // Should be unreachable
    }
    
    void CastExpression::typeCheck(diagnostic::Diagnostics& diag, bool& exit)
    {
    }

    bool CastExpression::triviallyImplicitCast(diagnostic::Diagnostics& diag, Type* destType)
    {
        return mValue->triviallyImplicitCast(diag, destType);
    }
}