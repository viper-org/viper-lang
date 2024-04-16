// Copyright 2024 solar-mist


#ifndef VIPER_FRAMEWORK_PARSER_AST_EXPRESSION_UNARY_EXPRESSION_H
#define VIPER_FRAMEWORK_PARSER_AST_EXPRESSION_UNARY_EXPRESSION_H 1

#include "parser/ast/Node.h"

#include "lexer/Token.h"

namespace parser
{
    class UnaryExpression : public ASTNode
    {
    public:
        enum class Operator
        {
            Negate
        };

        UnaryExpression(ASTNodePtr operand, lexing::TokenType op);
        UnaryExpression(ASTNodePtr operand, Operator op);

        vipir::Value* emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope) override;

    private:
        ASTNodePtr mOperand;
        Operator mOperator;
    };

    using UnaryExpressionPtr = std::unique_ptr<UnaryExpression>;
}

#endif // VIPER_FRAMEWORK_PARSER_AST_EXPRESSION_UNARY_EXPRESSION_H