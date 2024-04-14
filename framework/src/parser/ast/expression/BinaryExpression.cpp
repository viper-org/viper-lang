// Copyright 2024 solar-mist


#include "parser/ast/expression/BinaryExpression.h"

#include <vipir/Module.h>
#include <vipir/IR/Instruction/BinaryInst.h>
#include <vipir/IR/Instruction/StoreInst.h>

namespace parser
{
    BinaryExpression::BinaryExpression(ASTNodePtr left, lexing::TokenType tokenType, ASTNodePtr right)
        : mLeft(std::move(left))
        , mRight(std::move(right))
    {
        switch (tokenType)
        {
            case lexing::TokenType::Plus:
                mOperator = Operator::Add;
                break;
            case lexing::TokenType::Minus:
                mOperator = Operator::Sub;
                break;

            case lexing::TokenType::DoubleEquals:
                mOperator = Operator::Equal;
                break;

            case lexing::TokenType::Equals:
                mOperator = Operator::Assign;
                break;
                
            default:
                break;
        }
    }

    BinaryExpression::BinaryExpression(ASTNodePtr left, Operator op, ASTNodePtr right)
        : mLeft(std::move(left))
        , mOperator(op)
        , mRight(std::move(right))
    {
    }

    vipir::Value* BinaryExpression::emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope)
    {
        vipir::Value* left  = mLeft->emit(builder, module, scope);
        vipir::Value* right = mRight->emit(builder, module, scope);

        switch (mOperator)
        {
            case Operator::Add:
                return builder.CreateAdd(left, right);
            case Operator::Sub:
                return builder.CreateSub(left, right);

            case Operator::Equal:
                return builder.CreateCmpEQ(left, right);

            case Operator::Assign:
            {
                vipir::Value* pointerOperand = vipir::getPointerOperand(left);

                vipir::Instruction* instruction = static_cast<vipir::Instruction*>(left);
                instruction->eraseFromParent();

                return builder.CreateStore(pointerOperand, right);
            }
        }
    }
}