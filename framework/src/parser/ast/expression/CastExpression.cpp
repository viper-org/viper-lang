// Copyright 2024 solar-mist


#include "parser/ast/expression/CastExpression.h"

#include <vipir/IR/Instruction/PtrCastInst.h>
#include <vipir/IR/Instruction/SExtInst.h>
#include <vipir/IR/Instruction/TruncInst.h>

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
        if (mType->isPointerType() && mOperand->getType()->isPointerType())
        {
            return builder.CreatePtrCast(operand, mType->getVipirType());
        }
        else if (mType->isIntegerType() && mOperand->getType()->isIntegerType())
        {
            if (mType->getSize() > mOperand->getType()->getSize())
            {
                return builder.CreateSExt(operand, mType->getVipirType());
            }
            else
            {
                return builder.CreateTrunc(operand, mType->getVipirType());
            }
        }
        return nullptr;
    }
}