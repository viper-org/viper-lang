// Copyright 2024 solar-mist


#include "parser/ast/expression/BinaryExpression.h"

#include <vipir/Module.h>
#include <vipir/IR/Instruction/BinaryInst.h>
#include <vipir/IR/Instruction/StoreInst.h>

#include <cassert>

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

            case lexing::TokenType::Pipe:
                mOperator = Operator::BitwiseOr;
                break;
            case lexing::TokenType::Ampersand:
                mOperator = Operator::BitwiseAnd;
                break;
            case lexing::TokenType::Caret:
                mOperator = Operator::BitwiseXor;
                break;

            case lexing::TokenType::DoubleEquals:
                mOperator = Operator::Equal;
                break;
            case lexing::TokenType::BangEquals:
                mOperator = Operator::NotEqual;
                break;

            case lexing::TokenType::LessThan:
                mOperator = Operator::LessThan;
                break;
            case lexing::TokenType::GreaterThan:
                mOperator = Operator::GreaterThan;
                break;

            case lexing::TokenType::LessEqual:
                mOperator = Operator::LessEqual;
                break;
            case lexing::TokenType::GreaterEqual:
                mOperator = Operator::GreaterEqual;
                break;

            case lexing::TokenType::Equals:
                mOperator = Operator::Assign;
                break;
            case lexing::TokenType::PlusEquals:
                mOperator = Operator::AddAssign;
                break;
            case lexing::TokenType::MinusEquals:
                mOperator = Operator::SubAssign;
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

            case Operator::BitwiseOr:
                return builder.CreateBWOr(left, right);
            case Operator::BitwiseAnd:
                return builder.CreateBWAnd(left, right);
            case Operator::BitwiseXor:
                return builder.CreateBWXor(left, right);

            case Operator::Equal:
                return builder.CreateCmpEQ(left, right);
            case Operator::NotEqual:
                return builder.CreateCmpNE(left, right);

            case Operator::LessThan:
                return builder.CreateCmpLT(left, right);
            case Operator::GreaterThan:
                return builder.CreateCmpGT(left, right);

            case Operator::LessEqual:
                return builder.CreateCmpLE(left, right);
            case Operator::GreaterEqual:
                return builder.CreateCmpGE(left, right);

            case Operator::Assign:
            {
                vipir::Value* pointerOperand = vipir::getPointerOperand(left);

                vipir::Instruction* instruction = static_cast<vipir::Instruction*>(left);
                instruction->eraseFromParent();

                return builder.CreateStore(pointerOperand, right);
            }
            case Operator::AddAssign:
            {
                vipir::Value* pointerOperand = vipir::getPointerOperand(left);
                assert(pointerOperand != nullptr); // TODO: Proper error report

                vipir::Value* add = builder.CreateAdd(left, right);
                return builder.CreateStore(pointerOperand, add);
            }
            case Operator::SubAssign:
            {
                vipir::Value* pointerOperand = vipir::getPointerOperand(left);
                assert(pointerOperand != nullptr); // TODO: Proper error report

                vipir::Value* sub = builder.CreateSub(left, right);
                return builder.CreateStore(pointerOperand, sub);
            }
        }
    }
}