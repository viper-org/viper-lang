// Copyright 2024 solar-mist

#include "parser/ast/expression/UnaryExpression.h"

#include "type/PointerType.h"

#include <vipir/Module.h>

#include <vipir/IR/Function.h>
#include <vipir/IR/Instruction/UnaryInst.h>
#include <vipir/IR/Instruction/LoadInst.h>
#include <vipir/IR/Instruction/GEPInst.h>
#include <vipir/IR/Instruction/AddrInst.h>

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
                break;

            case lexer::TokenType::Star:
                mOperator = Operator::Indirection;
                break;

            case lexer::TokenType::Ampersand:
                mOperator = Operator::AddressOf;
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

            case parser::UnaryExpression::Operator::Indirection:
                return builder.CreateLoad(operand);

            case parser::UnaryExpression::Operator::AddressOf:
            {
                if (dynamic_cast<vipir::Function*>(operand)) return builder.CreateAddrOf(operand);
                auto pointerOperand = vipir::getPointerOperand(operand);
                auto instruction = static_cast<vipir::Instruction*>(operand);
                instruction->eraseFromParent();

                if (dynamic_cast<vipir::GEPInst*>(pointerOperand))
                {
                    return pointerOperand;
                }

                return builder.CreateAddrOf(pointerOperand);
            }

            default:
                break;
        }
        return nullptr; // Unreachable
    }

    void UnaryExpression::semanticCheck(diagnostic::Diagnostics& diag, bool& exit, bool statement)
    {
        mOperand->semanticCheck(diag, exit, false);
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
                    mType = Type::Get("error-type");
                }
                else
                {
                    mType = mOperand->getType();
                }
                break;

            case Operator::Indirection:
                if (!mOperand->getType()->isPointerType())
                {
                    diag.reportCompilerError(mErrorToken.getStartLocation(), mErrorToken.getEndLocation(),
                        std::format("No match for '{}operator{}{} with type '{}{}{}'",
                            fmt::bold, mErrorToken.getName(), fmt::defaults,
                            fmt::bold, mOperand->getType()->getName(), fmt::defaults));
                    exit = true;
                    mType = Type::Get("error-type");
                }
                else
                {
                    mType = static_cast<PointerType*>(mOperand->getType())->getPointeeType();
                }
                break;

            case Operator::AddressOf:
                mType = PointerType::Get(mOperand->getType());
                break;
        }
    }

    bool UnaryExpression::triviallyImplicitCast(diagnostic::Diagnostics& diag, Type* destType) 
    {
        return false;
    }
}