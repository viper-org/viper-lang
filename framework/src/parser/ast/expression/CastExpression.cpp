// Copyright 2024 solar-mist


#include "parser/ast/expression/CastExpression.h"

#include <vipir/IR/Instruction/PtrCastInst.h>

namespace parser
{
    CastExpression::CastExpression(ASTNodePtr operand, Type* destType)
        : mOperand(std::move(operand))
    {
        mType = destType;
    }

    vipir::Value* CastExpression::emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope)
    {
        vipir::Value* operand = mOperand->emit(builder, module, scope);
        if (mType->isPointerType())
        {
            return builder.CreatePtrCast(operand, mType->getVipirType());
        }
        return nullptr;
    }
}