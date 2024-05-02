// Copyright 2024 solar-mist


#ifndef VIPER_FRAMEWORK_PARSER_AST_EXPRESSION_CAST_EXPRESSION_H
#define VIPER_FRAMEWORK_PARSER_AST_EXPRESSION_CAST_EXPRESSION_H 1

#include "parser/ast/Node.h"

namespace parser
{
    class CastExpression : public ASTNode
    {
    public:
        CastExpression(ASTNodePtr operand, Type* destType);

        vipir::Value* emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope) override;

    private:
        ASTNodePtr mOperand;
    };

    using CastExpressionPtr = std::unique_ptr<CastExpression>;
}

#endif // VIPER_FRAMEWORK_PARSER_AST_EXPRESSION_CAST_EXPRESSION_H