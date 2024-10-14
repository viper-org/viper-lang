#ifndef VIPER_FRAMEWORK_PARSER_AST_EXPRESSION_BINARY_EXPRESSION_H
#define VIPER_FRAMEWORK_PARSER_AST_EXPRESSION_BINARY_EXPRESSION_H 1

#include "parser/ast/ASTNode.h"

#include <memory>

namespace parser
{
    class BinaryExpression : public ASTNode
    {
    public:
        enum class Operator 
        {
            Add,
            Sub,
            Mul,
            Div,
        };

        BinaryExpression(Scope* scope, ASTNodePtr left, lexer::TokenType operatorToken, ASTNodePtr right, lexer::Token token);

        virtual vipir::Value* codegen(vipir::IRBuilder& builder, vipir::Module& module, diagnostic::Diagnostics& diag) override;

        virtual void typeCheck(diagnostic::Diagnostics& diag, bool& exit) override;
        virtual bool implicitCast(diagnostic::Diagnostics& diag, Type* destType) override;
        virtual bool triviallyImplicitCast(diagnostic::Diagnostics& diag, Type* destType) override;

    private:
        ASTNodePtr mLeft;
        ASTNodePtr mRight;
        Operator mOperator;
    };
    using BinaryExpressionPtr = std::unique_ptr<BinaryExpression>;
}

#endif // VIPER_FRAMEWORK_PARSER_AST_EXPRESSION_BINARY_EXPRESSION_H
