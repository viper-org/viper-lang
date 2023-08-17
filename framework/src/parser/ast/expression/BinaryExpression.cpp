// Copyright 2023 solar-mist


#include "parser/ast/expression/BinaryExpression.h"

#include <vipir/IR/Instruction/BinOpInst.h>

namespace parsing
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

    vipir::Value* BinaryExpression::emit(vipir::Builder& builder, vipir::Module& module)
    {
        vipir::Value* left  = mLeft->emit(builder, module);
        vipir::Value* right = mRight->emit(builder, module);

        switch (mOperator)
        {
            case Operator::Add:
                return builder.CreateAdd(left, right);
        }
    }
}