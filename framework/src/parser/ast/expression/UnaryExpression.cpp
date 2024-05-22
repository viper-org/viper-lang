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
    UnaryExpression::UnaryExpression(ASTNodePtr operand, lexing::TokenType tokenType, bool postfix)
        : mOperand(std::move(operand))
    {
        switch(tokenType)
        {
            case lexing::TokenType::DoublePlus:
                mOperator = postfix ? Operator::PostfixDoublePlus : Operator::PrefixDoublePlus;
                mType = mOperand->getType();
                break;

            case lexing::TokenType::DoubleMinus:
                mOperator = postfix ? Operator::PostfixDoubleMinus : Operator::PrefixDoubleMinus;
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
    }

    UnaryExpression::UnaryExpression(ASTNodePtr operand, Operator op)
        : mOperand(std::move(operand))
        , mOperator(op)
    {
    }

    vipir::Value* UnaryExpression::emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope, diagnostic::Diagnostics& diag)
    {
        vipir::Value* operand = mOperand->emit(builder, module, scope, diag);

        switch(mOperator)
        {
            case Operator::PrefixDoublePlus:
                return builder.CreateAdd(operand, vipir::ConstantInt::Get(module, 1, mType->getVipirType()));

            case Operator::PrefixDoubleMinus:
                return builder.CreateSub(operand, vipir::ConstantInt::Get(module, 1, mType->getVipirType()));

            case Operator::PostfixDoublePlus:
            {
                vipir::Value* ptr = vipir::getPointerOperand(operand);
                vipir::Value* load = builder.CreateLoad(ptr);
                vipir::Value* add = builder.CreateAdd(operand, vipir::ConstantInt::Get(module, 1, mType->getVipirType()));
                builder.CreateStore(ptr, add);
                return load;
            }
            case Operator::PostfixDoubleMinus:
            {
                vipir::Value* ptr = vipir::getPointerOperand(operand);
                vipir::Value* load = builder.CreateLoad(ptr);
                vipir::Value* add = builder.CreateSub(operand, vipir::ConstantInt::Get(module, 1, mType->getVipirType()));
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
}