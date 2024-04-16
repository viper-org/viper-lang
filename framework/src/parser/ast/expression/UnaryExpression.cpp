// Copyright 2024 solar-mist


#include "parser/ast/expression/UnaryExpression.h"

#include <vipir/IR/Instruction/UnaryInst.h>

namespace parser
{
    UnaryExpression::UnaryExpression(ASTNodePtr operand, lexing::TokenType tokenType)
        : mOperand(std::move(operand))
    {
        switch(tokenType)
        {
            case lexing::TokenType::Minus:
                mOperator = Operator::Negate;
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
        }

        return nullptr;
    }
}