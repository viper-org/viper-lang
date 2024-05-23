// Copyright 2024 solar-mist


#include "parser/ast/expression/UnaryExpression.h"

#include "type/PointerType.h"

#include <vipir/IR/Constant/ConstantInt.h>
#include <vipir/IR/Instruction/BinaryInst.h>
#include <vipir/IR/Instruction/UnaryInst.h>
#include <vipir/IR/Instruction/AddrInst.h>
#include <vipir/IR/Instruction/LoadInst.h>
#include <vipir/IR/Instruction/GEPInst.h>

#include <vipir/Module.h>
#include <iostream>

namespace parser
{
    UnaryExpression::UnaryExpression(ASTNodePtr operand, lexing::Token operatorToken, bool postfix)
        : mOperand(std::move(operand))
        , mPostfix(postfix)
    {
        switch(operatorToken.getTokenType())
        {
            case lexing::TokenType::DoublePlus:
                mOperator = postfix ? Operator::PostIncrement : Operator::PreIncrement;
                mType = mOperand->getType();
                break;

            case lexing::TokenType::DoubleMinus:
                mOperator = postfix ? Operator::PostDecrement : Operator::PreDecrement;
                mType = mOperand->getType();
                break;

            case lexing::TokenType::Minus:
                mOperator = Operator::Negate;
                mType = mOperand->getType();
                break;

            case lexing::TokenType::Tilde:
                mOperator = Operator::BitwiseNot;
                mType = mOperand->getType();
                break;

            case lexing::TokenType::Ampersand:
                mOperator = Operator::AddressOf;
                mType = PointerType::Create(mOperand->getType());
                break;
            case lexing::TokenType::Star:
                mOperator = Operator::Indirection;
                mType = static_cast<PointerType*>(mOperand->getType())->getBaseType();
                break;

            default:
                break;
        }
        mPreferredDebugToken = std::move(operatorToken);
    }

    void UnaryExpression::typeCheck(Scope* scope, diagnostic::Diagnostics& diag)
    {
        switch (mOperator)
        {
            case Operator::PreIncrement:
            case Operator::PreDecrement:
            case Operator::PostIncrement:
            case Operator::PostDecrement:
                if (!(mType->isIntegerType() || mType->isPointerType()))
                {
                    diag.compilerError(mPreferredDebugToken.getStart(), mPreferredDebugToken.getEnd(),
                        std::format("No match for '{}operator{}{}' with type '{}{}{}'",
                        fmt::bold, mPreferredDebugToken.getId(), fmt::defaults,
                        fmt::bold, mType->getName(),             fmt::defaults));
                }
                break;
            
            case Operator::Indirection:
                if (!mType->isPointerType())
                {
                    diag.compilerError(mPreferredDebugToken.getStart(), mPreferredDebugToken.getEnd(),
                        std::format("No match for '{}operator*{}' with type '{}{}{}'",
                        fmt::bold, fmt::defaults,
                        fmt::bold, mType->getName(), fmt::defaults));
                }
                break;

            case Operator::Negate:
            case Operator::BitwiseNot:
                if (!mOperand->getType()->isIntegerType())
                {
                    diag.compilerError(mPreferredDebugToken.getStart(), mPreferredDebugToken.getEnd(),
                        std::format("No match for '{}operator{}{} with type '{}{}{}'",
                        fmt::bold, mPreferredDebugToken.getId(),    fmt::defaults,
                        fmt::bold, mOperand->getType()->getName(),  fmt::defaults));
                }
                break;
            
            default:
                break; // maybe check for address-of actually being a variable here
        }

        mOperand->typeCheck(scope, diag);
    }

    vipir::Value* UnaryExpression::emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope, diagnostic::Diagnostics& diag)
    {
        vipir::Value* operand = mOperand->emit(builder, module, scope, diag);

        switch(mOperator)
        {
            case Operator::PreIncrement:
                if (mType->isPointerType())
                    return builder.CreateGEP(operand, vipir::ConstantInt::Get(module, 1, vipir::Type::GetIntegerType(32))); // TODO: fix GEP 123 +1 = -1
                else
                    return builder.CreateAdd(operand, vipir::ConstantInt::Get(module, 1, mType->getVipirType()));

            case Operator::PreDecrement:
                if (mType->isPointerType())
                    return builder.CreateGEP(operand, vipir::ConstantInt::Get(module, -1, vipir::Type::GetIntegerType(32)));
                else
                    return builder.CreateSub(operand, vipir::ConstantInt::Get(module, -1, mType->getVipirType()));

            case Operator::PostIncrement:
            {
                checkAssignmentLvalue(operand, diag);
                vipir::Value* ptr = vipir::getPointerOperand(operand);
                vipir::Value* load = builder.CreateLoad(ptr);
                vipir::Value* add = nullptr;
                if (mType->isPointerType())
                    add = builder.CreateGEP(operand, vipir::ConstantInt::Get(module, 1, vipir::Type::GetIntegerType(32)));
                else
                    add = builder.CreateAdd(operand, vipir::ConstantInt::Get(module, 1, mType->getVipirType()));
                builder.CreateStore(ptr, add);
                return load;
            }
            case Operator::PostDecrement:
            {
                checkAssignmentLvalue(operand, diag);
                vipir::Value* ptr = vipir::getPointerOperand(operand);
                vipir::Value* load = builder.CreateLoad(ptr);
                vipir::Value* add = nullptr;
                if (mType->isPointerType())
                    add = builder.CreateGEP(operand, vipir::ConstantInt::Get(module, -1, vipir::Type::GetIntegerType(32)));
                else
                    add = builder.CreateSub(operand, vipir::ConstantInt::Get(module, 1, mType->getVipirType()));
                builder.CreateStore(ptr, add);
                return load;
            }
            case Operator::Negate:
                return builder.CreateNeg(operand);

            case Operator::BitwiseNot:
                return builder.CreateNot(operand);

            case Operator::AddressOf:
            {
                vipir::Value* pointerOperand = vipir::getPointerOperand(operand);

                vipir::Instruction* instruction = static_cast<vipir::Instruction*>(operand);
                instruction->eraseFromParent();

                if (dynamic_cast<vipir::GEPInst*>(pointerOperand))
                {
                    return pointerOperand;
                }

                return builder.CreateAddrOf(pointerOperand);
            }
            case Operator::Indirection:
            {
                return builder.CreateLoad(operand);
            }
        }

        return nullptr;
    }

    void UnaryExpression::checkAssignmentLvalue(vipir::Value* value, diagnostic::Diagnostics& diag)
    {
        vipir::Value* pointerOperand = vipir::getPointerOperand(value);
        if (pointerOperand == nullptr)
        {
            diag.compilerError(mPreferredDebugToken.getStart(), mPreferredDebugToken.getEnd(),
                std::format("lvalue required as {} operand of assignment",
                mPostfix ? "left" : "right"));
        }
    }
}