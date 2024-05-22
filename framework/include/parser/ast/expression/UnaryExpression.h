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
            Negate,
            
            BitwiseNot,

            AddressOf, Indirection,
        };

        UnaryExpression(ASTNodePtr operand, lexing::Token operatorToken);

        void typeCheck(Scope* scope, diagnostic::Diagnostics& diag) override;
        vipir::Value* emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope, diagnostic::Diagnostics& diag) override;

    private:
        ASTNodePtr mOperand;
        Operator mOperator;
    };

    using UnaryExpressionPtr = std::unique_ptr<UnaryExpression>;
}

#endif // VIPER_FRAMEWORK_PARSER_AST_EXPRESSION_UNARY_EXPRESSION_H