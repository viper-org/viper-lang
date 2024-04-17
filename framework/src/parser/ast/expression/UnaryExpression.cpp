// Copyright 2024 solar-mist


#include "parser/ast/expression/UnaryExpression.h"

#include "type/PointerType.h"

#include <vipir/IR/Instruction/UnaryInst.h>
#include <vipir/IR/Instruction/AddrInst.h>
#include <vipir/IR/Instruction/LoadInst.h>

#include <vipir/Module.h>

namespace parser
{
    UnaryExpression::UnaryExpression(ASTNodePtr operand, lexing::TokenType tokenType)
        : mOperand(std::move(operand))
    {
        switch(tokenType)
        {
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
                break; // TODO: Error
        }
    }

    UnaryExpression::UnaryExpression(ASTNodePtr operand, Operator op)
        : mOperand(std::move(operand))
        , mOperator(op)
    {
    }

    vipir::Value* UnaryExpression::emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope)
    {
        vipir::Value* operand = mOperand->emit(builder, module, scope);

        switch(mOperator)
        {
            case Operator::Negate:
                return builder.CreateNeg(operand);

            case Operator::BitwiseNot:
                return builder.CreateNot(operand);

            case Operator::AddressOf:
            {
                vipir::Value* pointerOperand = vipir::getPointerOperand(operand);

                vipir::Instruction* instruction = static_cast<vipir::Instruction*>(operand);
                instruction->eraseFromParent();

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