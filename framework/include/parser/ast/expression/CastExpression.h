// Copyright 2024 solar-mist


#ifndef VIPER_FRAMEWORK_PARSER_AST_EXPRESSION_CAST_EXPRESSION_H
#define VIPER_FRAMEWORK_PARSER_AST_EXPRESSION_CAST_EXPRESSION_H 1

#include "parser/ast/Node.h"

#include "lexer/Token.h"

namespace parser
{
    class CastExpression : public ASTNode
    {
    public:
        CastExpression(ASTNodePtr operand, Type* destType, lexing::Token token);

        void typeCheck(Scope* scope, diagnostic::Diagnostics& diag) override;
        vipir::Value* emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope, diagnostic::Diagnostics& diag) override;

    private:
        ASTNodePtr mOperand;
        lexing::Token mToken;
    };

    using CastExpressionPtr = std::unique_ptr<CastExpression>;
}

#endif // VIPER_FRAMEWORK_PARSER_AST_EXPRESSION_CAST_EXPRESSION_H