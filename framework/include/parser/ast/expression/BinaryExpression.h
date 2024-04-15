// Copyright 2024 solar-mist


#ifndef VIPER_FRAMEWORK_PARSER_AST_EXPRESSION_BINARY_EXPRESSION_H
#define VIPER_FRAMEWORK_PARSER_AST_EXPRESSION_BINARY_EXPRESSION_H 1

#include "parser/ast/Node.h"

#include "lexer/Token.h"

namespace parser
{
    class BinaryExpression : public ASTNode
    {
    public:
        enum class Operator
        {
            Add,
            Sub,

            Equal, NotEqual,

            Assign,
        };

        BinaryExpression(ASTNodePtr left, lexing::TokenType op, ASTNodePtr right);
        BinaryExpression(ASTNodePtr left, Operator op, ASTNodePtr right);

        vipir::Value* emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope) override;

    private:
        ASTNodePtr mLeft;
        Operator mOperator;
        ASTNodePtr mRight;
    };

    using BinaryExpressionPtr = std::unique_ptr<BinaryExpression>;
}

#endif // VIPER_FRAMEWORK_PARSER_AST_EXPRESSION_BINARY_EXPRESSION_H