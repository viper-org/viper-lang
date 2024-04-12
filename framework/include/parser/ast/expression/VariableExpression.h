// Copyright 2024 solar-mist

#ifndef VIPER_FRAMEWORK_PARSER_AST_EXPRESSION_VARIABLE_EXPRESSION_H
#define VIPER_FRAMEWORK_PARSER_AST_EXPRESSION_VARIABLE_EXPRESSION_H 1

#include "parser/ast/Node.h"

namespace parser
{
    class VariableExpression : public ASTNode
    {
    public:
        VariableExpression(std::string&& name);

        vipir::Value* emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope) override;

    private:
        std::string mName;
    };
    using VariableExpressionPtr = std::unique_ptr<VariableExpression>;
}

#endif // VIPER_FRAMEWORK_PARSER_AST_EXPRESSION_VARIABLE_EXPRESSION_H