// Copyright 2023 solar-mist


#ifndef VIPER_FRAMEWORK_PARSER_AST_EXPRESSION_UNARY_H
#define VIPER_FRAMEWORK_PARSER_AST_EXPRESSION_UNARY_H 1

#include "parser/ast/Node.h"

#include "lexer/Token.h"

namespace parsing
{
    class UnaryExpression : public ASTNode
    {
    public:
        enum class Operator
        {
            AddressOf,
            Dereference,
        };

        UnaryExpression(ASTNodePtr operand, lexing::TokenType op);

        vipir::Value* emit(vipir::Builder& builder, vipir::Module& module, Environment* scope) override;

    private:
        ASTNodePtr mOperand;
        Operator mOperator;
    };

    using UnaryExpressionPtr = std::unique_ptr<UnaryExpression>;
}

#endif