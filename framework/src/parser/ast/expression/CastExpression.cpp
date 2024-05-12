// Copyright 2024 solar-mist


#include "parser/ast/expression/CastExpression.h"

#include <vipir/IR/Instruction/PtrCastInst.h>
#include <vipir/IR/Instruction/SExtInst.h>
#include <vipir/IR/Instruction/TruncInst.h>
#include <vipir/IR/Instruction/IntToPtrInst.h>
#include <vipir/IR/Instruction/PtrToIntInst.h>

namespace parser
{
    CastExpression::CastExpression(ASTNodePtr operand, Type* destType, lexing::Token token)
        : mOperand(std::move(operand))
        , mToken(std::move(token))
    {
        mType = destType;
    }

    vipir::Value* CastExpression::emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope, diagnostic::Diagnostics& diag)
    {
        vipir::Value* operand = mOperand->emit(builder, module, scope, diag);
        if (mType->isPointerType())
        {
            if (mOperand->getType()->isPointerType())
            {
                return builder.CreatePtrCast(operand, mType->getVipirType());
            }
            else if (mOperand->getType()->isIntegerType())
            {
                vipir::Value* ptrtoint = builder.CreateIntToPtr(operand, mType->getVipirType());
                if (mOperand->getType()->getSize() > mType->getSize())
                {
                    return builder.CreateTrunc(ptrtoint, mType->getVipirType());
                }
                return ptrtoint;
            }
        }
        else if (mType->isIntegerType())
        {
            if (mOperand->getType()->isIntegerType())
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
            else if (mOperand->getType()->isPointerType())
            {
                if (mOperand->getType()->getSize() < mType->getSize())
                {
                    vipir::Type* pointerIntegerType = vipir::Type::GetIntegerType(mOperand->getType()->getVipirType()->getSizeInBits());
                    operand = builder.CreateSExt(operand, pointerIntegerType);
                }
                return builder.CreatePtrToInt(operand, mType->getVipirType());
            }
        }

        diag.compilerError(mToken.getStart(), mToken.getEnd(), std::format("value has type '{}{}{}' which cannot be converted to '{}{}{}",
            fmt::bold, mOperand->getType()->getName(), fmt::defaults, fmt::bold, mType->getName(), fmt::defaults));
    }
}