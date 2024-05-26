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
            Add, Sub,
            Mul, Div,

            BitwiseOr, BitwiseAnd, BitwiseXor,

            Equal, NotEqual,
            LessThan, GreaterThan,
            LessEqual, GreaterEqual,

            Assign,
            AddAssign, SubAssign,

            ArrayAccess,
        };

        BinaryExpression(ASTNodePtr left, lexing::Token operatorToken, ASTNodePtr right);

        void typeCheck(Scope* scope, diagnostic::Diagnostics& diag) override;
        vipir::Value* emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope, diagnostic::Diagnostics& diag) override;

    private:
        ASTNodePtr mLeft;
        Operator mOperator;
        lexing::Token mToken;
        ASTNodePtr mRight;

        void checkAssignmentLvalue(vipir::Value* pointer, diagnostic::Diagnostics& diag);
    };

    using BinaryExpressionPtr = std::unique_ptr<BinaryExpression>;
}

#endif // VIPER_FRAMEWORK_PARSER_AST_EXPRESSION_BINARY_EXPRESSION_H