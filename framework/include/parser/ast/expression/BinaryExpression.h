// Copyright 2023 solar-mist


#ifndef VIPER_FRAMEWORK_PARSER_AST_EXPRESSION_BINARY_H
#define VIPER_FRAMEWORK_PARSER_AST_EXPRESSION_BINARY_H 1

#include "parser/ast/Node.h"

#include "lexer/Token.h"

namespace parsing
{
    class BinaryExpression : public ASTNode
    {
    public:
        enum class Operator
        {
            Add,
            Sub,

            Assign,
        };

        BinaryExpression(ASTNodePtr left, lexing::TokenType op, ASTNodePtr right);
        BinaryExpression(ASTNodePtr left, Operator op, ASTNodePtr right);

        vipir::Value* emit(vipir::Builder& builder, vipir::Module& module, Environment* scope) override;

    private:
        ASTNodePtr mLeft;
        Operator mOperator;
        ASTNodePtr mRight;
    };

    using BinaryExpressionPtr = std::unique_ptr<BinaryExpression>;
}

#endif