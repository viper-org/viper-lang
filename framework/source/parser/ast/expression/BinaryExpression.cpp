// Copyright 2024 solar-mist

#include "parser/ast/expression/BinaryExpression.h"

#include "type/IntegerType.h"

#include <vipir/IR/Instruction/BinaryInst.h>

namespace parser
{
	BinaryExpression::BinaryExpression(Scope* scope, ASTNodePtr left, lexer::TokenType operatorToken, ASTNodePtr right, lexer::Token token)
		: ASTNode(scope, token)
		, mLeft(std::move(left))
		, mRight(std::move(right))
	{
		switch (operatorToken)
		{
            case lexer::TokenType::Plus:
                mOperator = Operator::Add;
                mType = mLeft->getType();
                break;
            case lexer::TokenType::Minus:
                mOperator = Operator::Sub;
                mType = mLeft->getType();
                break;

            case lexer::TokenType::Star:
                mOperator = Operator::Mul;
                mType = mLeft->getType();
                break;
            case lexer::TokenType::Slash:
                mOperator = Operator::Div;
                mType = mLeft->getType();
                break;
            default:
                break; // Unreachable
		}
	}

    vipir::Value* BinaryExpression::codegen(vipir::IRBuilder& builder, vipir::Module& module, diagnostic::Diagnostics& diag)
    {
        vipir::Value* left = mLeft->codegen(builder, module, diag);
        vipir::Value* right = mRight->codegen(builder, module, diag);

        switch (mOperator) 
        {
            case parser::BinaryExpression::Operator::Add:
                return builder.CreateAdd(left, right);
            case parser::BinaryExpression::Operator::Sub:
                return builder.CreateSub(left, right);

            case parser::BinaryExpression::Operator::Mul:
                if (mType->isIntegerType())
                {
                    if (static_cast<IntegerType*>(mType)->isSigned())
                        return builder.CreateSMul(left, right);
                    
                    return builder.CreateUMul(left, right);
                }

                diag.reportCompilerError(mErrorToken.getStartLocation(), mErrorToken.getEndLocation(), "Can't multiply a non-integer type");
                std::exit(1);
                break;

            case parser::BinaryExpression::Operator::Div:
                if (mType->isIntegerType())
                {
                    if (static_cast<IntegerType*>(mType)->isSigned())
                        return builder.CreateSDiv(left, right);
                    
                    return builder.CreateUDiv(left, right);
                }

                diag.reportCompilerError(mErrorToken.getStartLocation(), mErrorToken.getEndLocation(), "Can't divide a non-integer type");
                std::exit(1);
                break;
            default:
                break;
        }
        return nullptr; // Unreachable
    }

    void BinaryExpression::typeCheck(diagnostic::Diagnostics& diag, bool& exit)
    {
        mLeft->typeCheck(diag, exit);
        mRight->typeCheck(diag, exit);

        switch (mOperator) 
        {
            case parser::BinaryExpression::Operator::Add:
            case parser::BinaryExpression::Operator::Sub:
            case parser::BinaryExpression::Operator::Mul:
            case parser::BinaryExpression::Operator::Div:
                if (mLeft->getType() != mRight->getType() && mLeft->getType()->isIntegerType() && mRight->getType()->isIntegerType())
                {
                    if (mLeft->getType()->getSize() > mRight->getType()->getSize()) // lhs > rhs
                    {
                        if (mRight->implicitCast(diag, mLeft->getType()))
                        {
                            mRight = Cast(mRight, mLeft->getType());
                        }
                        mType = mLeft->getType();
                    }
                    else // rhs > lhs
                    {
                        if (mLeft->implicitCast(diag, mRight->getType()))
                        {
                            mLeft = Cast(mLeft, mRight->getType());
                        }
                        mType = mRight->getType();
                    }
                }
                if (mLeft->getType() != mRight->getType() || !mLeft->getType()->isIntegerType())
                {
                    diag.reportCompilerError(mErrorToken.getStartLocation(), mErrorToken.getEndLocation(), std::format("No match for '{}operator{}{} with types '{}{}{}' and '{}{}{}'",
                        fmt::bold, mErrorToken.getName(), fmt::defaults,
                        fmt::bold, mLeft->getType()->getName(), fmt::defaults,
                        fmt::bold, mRight->getType()->getName(), fmt::defaults));
                    exit = true;
                }
                break;

            default:
                break;
        }
    }

    bool BinaryExpression::triviallyImplicitCast(diagnostic::Diagnostics& diag, Type* destType) 
    {
        return false;
    }
}