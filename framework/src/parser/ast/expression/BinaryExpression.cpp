// Copyright 2024 solar-mist


#include "parser/ast/expression/BinaryExpression.h"

#include "type/ArrayType.h"
#include "type/IntegerType.h"

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
        switch (mToken.getTokenType())
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
            case lexing::TokenType::Slash:
                mOperator = Operator::Div;
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

        mPreferredDebugToken = mToken;
    }

    void BinaryExpression::typeCheck(Scope* scope, diagnostic::Diagnostics& diag)
    {
        switch (mOperator)
        {
            case Operator::Add:
                if (mLeft->getType()->isPointerType())
                {
                    if (!mRight->getType()->isIntegerType())
                    {
                        diag.compilerError(mToken.getStart(), mToken.getEnd(), std::format("No match for '{}operator+{} with types '{}{}{}' and '{}{}{}",
                            fmt::bold, fmt::defaults,
                            fmt::bold, mLeft->getType()->getName(),  fmt::defaults,
                            fmt::bold, mRight->getType()->getName(), fmt::defaults));
                    }
                }
                if (mRight->getType()->isPointerType())
                {
                    if (!mRight->getType()->isIntegerType())
                    {
                        diag.compilerError(mToken.getStart(), mToken.getEnd(), std::format("No match for '{}operator+{} with types '{}{}{}' and '{}{}{}'",
                            fmt::bold, fmt::defaults,
                            fmt::bold, mLeft->getType()->getName(),  fmt::defaults,
                            fmt::bold, mRight->getType()->getName(), fmt::defaults));
                    }
                }
                break;
            case Operator::Sub:
            case Operator::Mul:
            case Operator::Div:
            case Operator::BitwiseAnd:
            case Operator::BitwiseOr:
            case Operator::BitwiseXor:
            case Operator::Equal:
            case Operator::NotEqual:
            case Operator::LessThan:
            case Operator::GreaterThan:
            case Operator::LessEqual:
            case Operator::GreaterEqual:
                if (mLeft->getType() != mRight->getType() || !mLeft->getType()->isIntegerType())
                {
                    diag.compilerError(mToken.getStart(), mToken.getEnd(), std::format("No match for '{}operator{}{} with types '{}{}{}' and '{}{}{}'",
                            fmt::bold, mToken.getId(),               fmt::defaults,
                            fmt::bold, mLeft->getType()->getName(),  fmt::defaults,
                            fmt::bold, mRight->getType()->getName(), fmt::defaults));
                }
                break;
            
            case Operator::AddAssign:
                if (mLeft->getType()->isPointerType())
                {
                    if (!mRight->getType()->isIntegerType() && mRight->getType() != mLeft->getType())
                    {
                        diag.compilerError(mToken.getStart(), mToken.getEnd(), std::format("No match for '{}operator+={} with types '{}{}{}' and '{}{}{}'",
                            fmt::bold, fmt::defaults,
                            fmt::bold, mLeft->getType()->getName(),  fmt::defaults,
                            fmt::bold, mRight->getType()->getName(), fmt::defaults));
                    }
                }
            case Operator::Assign:
            case Operator::SubAssign:
                if (mLeft->getType() != mRight->getType())
                {
                    diag.compilerError(mToken.getStart(), mToken.getEnd(), std::format("No match for '{}operator{}{} with types '{}{}{}' and '{}{}{}'",
                            fmt::bold, mToken.getId(),               fmt::defaults,
                            fmt::bold, mLeft->getType()->getName(),  fmt::defaults,
                            fmt::bold, mRight->getType()->getName(), fmt::defaults));
                }
                break;

            case Operator::ArrayAccess:
                if (!mLeft->getType()->isArrayType() || !mRight->getType()->isIntegerType())
                {
                    diag.compilerError(mToken.getStart(), mToken.getEnd(), std::format("No match for '{}operator[]{} with types '{}{}{}' and '{}{}{}'",
                            fmt::bold, fmt::defaults,
                            fmt::bold, mLeft->getType()->getName(),  fmt::defaults,
                            fmt::bold, mRight->getType()->getName(), fmt::defaults));
                }
                break;
        }

        mLeft->typeCheck(scope, diag);
        mRight->typeCheck(scope, diag);
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
                if (mType->isIntegerType())
                {
                    if (static_cast<IntegerType*>(mType)->isSigned())
                        return builder.CreateSMul(left, right);
                    return builder.CreateUMul(left, right);
                }
                break; // TODO: Error?

            case Operator::Div:
                if (mType->isIntegerType())
                {
                    if (static_cast<IntegerType*>(mType)->isSigned())
                        return builder.CreateSDiv(left, right);
                    return builder.CreateUDiv(left, right);
                }
                break; // TODO: Error?

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

                vipir::Value* add;
                if (left->getType()->isPointerType())
                {
                    add = builder.CreateGEP(left, right);
                }
                else
                {
                    add = builder.CreateAdd(left, right);
                }

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

        return nullptr; // unreachable, just to silence warnings
    }


    void BinaryExpression::checkAssignmentLvalue(vipir::Value* pointer, diagnostic::Diagnostics& diag)
    {
        if (pointer == nullptr)
        {
            diag.compilerError(mToken.getStart(), mToken.getEnd(), std::format("lvalue required as left operand of assignment"));
        }
    }
}
