// Copyright 2024 solar-mist


#include "parser/ast/expression/CastExpression.h"

#include "type/IntegerType.h"

#include <vipir/IR/Instruction/PtrCastInst.h>
#include <vipir/IR/Instruction/SExtInst.h>
#include <vipir/IR/Instruction/ZExtInst.h>
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
                if (mOperand->getType()->getSize() < mType->getSize())
                {
                    auto operandIntegerType = static_cast<IntegerType*>(mOperand->getType());
                    auto pointerIntegerType = vipir::Type::GetIntegerType(mType->getVipirType()->getSizeInBits());
                    if (operandIntegerType->isSigned())
                    {
                        operand = builder.CreateSExt(operand, pointerIntegerType);
                    }
                    else
                    {
                        operand = builder.CreateZExt(operand, pointerIntegerType);
                    }
                }
                return builder.CreateIntToPtr(operand, mType->getVipirType());
            }
        }
        else if (mType->isIntegerType())
        {
            auto integerType = static_cast<IntegerType*>(mType);
            if (mOperand->getType()->isIntegerType())
            {
                auto operandIntegerType = static_cast<IntegerType*>(mOperand->getType());
                
                if (mType->getSize() > mOperand->getType()->getSize())
                {
                    if (!operandIntegerType->isSigned())
                    {
                        return builder.CreateZExt(operand, mType->getVipirType());
                    }
                    return builder.CreateSExt(operand, mType->getVipirType());
                }
                else if (mType->getSize() < mOperand->getType()->getSize())
                {
                    return builder.CreateTrunc(operand, mType->getVipirType());
                }
                else if ((integerType->isSigned() && !operandIntegerType->isSigned()) ||
                         (!integerType->isSigned() && operandIntegerType->isSigned()))
                {
                    return operand;
                }
            }
            else if (mOperand->getType()->isPointerType())
            {
                auto ptrtoint = builder.CreatePtrToInt(operand, mType->getVipirType());

                if (mType->getSize() < mOperand->getType()->getSize())
                {
                    return builder.CreateTrunc(ptrtoint, mType->getVipirType());
                }
                return ptrtoint;
            }
        }

        diag.compilerError(mToken.getStart(), mToken.getEnd(), std::format("value has type '{}{}{}' which cannot be converted to '{}{}{}",
            fmt::bold, mOperand->getType()->getName(), fmt::defaults, fmt::bold, mType->getName(), fmt::defaults));
    }
}