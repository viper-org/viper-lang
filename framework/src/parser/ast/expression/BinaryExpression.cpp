// Copyright 2024 solar-mist


#include "parser/ast/expression/BinaryExpression.h"

#include "type/ArrayType.h"

#include <vipir/Module.h>
#include <vipir/IR/Instruction/BinaryInst.h>
#include <vipir/IR/Instruction/StoreInst.h>
#include <vipir/IR/Instruction/GEPInst.h>
#include <vipir/IR/Instruction/LoadInst.h>

#include <cassert>

namespace parser
{
    BinaryExpression::BinaryExpression(ASTNodePtr left, lexing::Token operatorToken, ASTNodePtr right)
        : mLeft(std::move(left))
        , mRight(std::move(right))
        , mToken(std::move(operatorToken))
    {
        switch (operatorToken.getTokenType())
        {
            case lexing::TokenType::Plus:
                mOperator = Operator::Add;
                mType = mLeft->getType();
                break;
            case lexing::TokenType::Minus:
                mOperator = Operator::Sub;
                mType = mLeft->getType();
                break;

            case lexing::TokenType::Star:
                mOperator = Operator::Mul;
                mType = mLeft->getType();
                break;

            case lexing::TokenType::Pipe:
                mOperator = Operator::BitwiseOr;
                mType = mLeft->getType();
                break;
            case lexing::TokenType::Ampersand:
                mOperator = Operator::BitwiseAnd;
                mType = mLeft->getType();
                break;
            case lexing::TokenType::Caret:
                mOperator = Operator::BitwiseXor;
                mType = mLeft->getType();
                break;

            case lexing::TokenType::DoubleEquals:
                mOperator = Operator::Equal;
                mType = Type::Get("bool");
                break;
            case lexing::TokenType::BangEquals:
                mOperator = Operator::NotEqual;
                mType = Type::Get("bool");
                break;

            case lexing::TokenType::LessThan:
                mOperator = Operator::LessThan;
                mType = Type::Get("bool");
                break;
            case lexing::TokenType::GreaterThan:
                mOperator = Operator::GreaterThan;
                mType = Type::Get("bool");
                break;

            case lexing::TokenType::LessEqual:
                mOperator = Operator::LessEqual;
                mType = Type::Get("bool");
                break;
            case lexing::TokenType::GreaterEqual:
                mOperator = Operator::GreaterEqual;
                mType = Type::Get("bool");
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

            case lexing::TokenType::LeftSquareBracket:
                mOperator = Operator::ArrayAccess;
                mType = static_cast<ArrayType*>(mLeft->getType())->getBaseType();
                break;

            default:
                break;
        }
    }

    vipir::Value* BinaryExpression::emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope, diagnostic::Diagnostics& diag)
    {
        vipir::Value* left  = mLeft->emit(builder, module, scope, diag);
        vipir::Value* right = mRight->emit(builder, module, scope, diag);

        switch (mOperator)
        {
            case Operator::Add:
                if (left->getType()->isPointerType())
                {
                    return builder.CreateGEP(left, right);
                }
                else if (right->getType()->isPointerType())
                {
                    return builder.CreateGEP(right, left);
                }
                return builder.CreateAdd(left, right);
            case Operator::Sub:
                return builder.CreateSub(left, right);

            case Operator::Mul:
                return builder.CreateIMul(left, right);

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
                checkAssignmentLvalue(pointerOperand, diag);

                vipir::Instruction* instruction = static_cast<vipir::Instruction*>(left);
                instruction->eraseFromParent();

                return builder.CreateStore(pointerOperand, right);
            }
            case Operator::AddAssign:
            {
                vipir::Value* pointerOperand = vipir::getPointerOperand(left);
                checkAssignmentLvalue(pointerOperand, diag);

                vipir::Value* add = builder.CreateAdd(left, right);
                return builder.CreateStore(pointerOperand, add);
            }
            case Operator::SubAssign:
            {
                vipir::Value* pointerOperand = vipir::getPointerOperand(left);
                checkAssignmentLvalue(pointerOperand, diag);

                vipir::Value* sub = builder.CreateSub(left, right);
                return builder.CreateStore(pointerOperand, sub);
            }

            case Operator::ArrayAccess:
            {
                vipir::Value* pointerOperand = vipir::getPointerOperand(left);

                vipir::Instruction* instruction = static_cast<vipir::Instruction*>(left);
                instruction->eraseFromParent();

                vipir::Value* gep = builder.CreateGEP(pointerOperand, right);

                return builder.CreateLoad(gep);
            }
        }
    }


    void BinaryExpression::checkAssignmentLvalue(vipir::Value* pointer, diagnostic::Diagnostics& diag)
    {
        if (pointer == nullptr)
        {
            diag.compilerError(mToken.getStart(), mToken.getEnd(), std::format("lvalue required as left operand of assignment"));
        }
    }
}