// Copyright 2024 solar-mist


#include "parser/ast/expression/UnaryExpression.h"

#include "type/PointerType.h"

#include <vipir/IR/Instruction/UnaryInst.h>
#include <vipir/IR/Instruction/AddrInst.h>
#include <vipir/IR/Instruction/LoadInst.h>
#include <vipir/IR/Instruction/GEPInst.h>

#include <vipir/Module.h>

namespace parser
{
    UnaryExpression::UnaryExpression(ASTNodePtr operand, lexing::Token operatorToken)
        : mOperand(std::move(operand))
    {
        switch(operatorToken.getTokenType())
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
                break;
        }
        mPreferredDebugToken = std::move(operatorToken);
    }

    void UnaryExpression::typeCheck(Scope* scope, diagnostic::Diagnostics& diag)
    {
        switch (mOperator)
        {
            case Operator::Indirection:
                if (!mType->isPointerType())
                {
                    diag.compilerError(mPreferredDebugToken.getStart(), mPreferredDebugToken.getEnd(), std::format("No match for '{}operator*{}' with type '{}{}{}'",
                        fmt::bold, fmt::defaults,
                        fmt::bold, mType->getName(), fmt::defaults));
                }

            case Operator::Negate:
            case Operator::BitwiseNot:
                if (!mOperand->getType()->isIntegerType())
                {
                    diag.compilerError(mPreferredDebugToken.getStart(), mPreferredDebugToken.getEnd(), std::format("No match for '{}operator{}{} with type '{}{}{}'",
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