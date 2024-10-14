// Copyright 2024 solar-mist

#include "parser/ast/expression/UnaryExpression.h"

#include <vipir/IR/Instruction/UnaryInst.h>

namespace parser
{
	UnaryExpression::UnaryExpression(Scope* scope, ASTNodePtr operand, lexer::TokenType operatorToken, bool postfix, lexer::Token token)
		: ASTNode(scope, token)
        , mOperand(std::move(operand))
        , mPostfix(postfix)
	{
		switch (operatorToken)
		{
            case lexer::TokenType::Minus:
                mOperator = Operator::Negate;
                mType = mOperand->getType();
                break;
            
            default:
                break; // Unreachable
        }
	}

    vipir::Value* UnaryExpression::codegen(vipir::IRBuilder& builder, vipir::Module& module, diagnostic::Diagnostics& diag)
    {
        vipir::Value* operand = mOperand->codegen(builder, module, diag);

        switch (mOperator) 
        {
            case parser::UnaryExpression::Operator::Negate:
                return builder.CreateNeg(operand);

            default:
                break;
        }
        return nullptr; // Unreachable
    }

    void UnaryExpression::typeCheck(diagnostic::Diagnostics& diag, bool& exit)
    {
        mOperand->typeCheck(diag, exit);

        switch (mOperator) 
        {
            case Operator::Negate:
                if (!mOperand->getType()->isIntegerType())
                {
                    diag.reportCompilerError(mErrorToken.getStartLocation(), mErrorToken.getEndLocation(),
                        std::format("No match for '{}operator{}{} with type '{}{}{}'",
                            fmt::bold, mErrorToken.getName(), fmt::defaults,
                            fmt::bold, mOperand->getType()->getName(), fmt::defaults));
                    exit = true;
                }
                mType = mOperand->getType();
                break;
        }
    }

    bool UnaryExpression::triviallyImplicitCast(diagnostic::Diagnostics& diag, Type* destType) 
    {
        return false;
    }
}