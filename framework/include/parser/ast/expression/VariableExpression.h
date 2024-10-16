// Copyright 2024 solar-mist

#ifndef VIPER_FRAMEWORK_PARSER_AST_EXPRESSION_VARIABLE_EXPRESSION_H
#define VIPER_FRAMEWORK_PARSER_AST_EXPRESSION_VARIABLE_EXPRESSION_H 1

#include "parser/ast/ASTNode.h"

#include <cstdint>
#include <memory>

namespace parser
{
    class VariableExpression : public ASTNode
    {
    friend class ::ASTNodeIntrospector;
    public:
        VariableExpression(Scope* scope, std::string name, lexer::Token token);

        virtual vipir::Value* codegen(vipir::IRBuilder& builder, vipir::Module& module, diagnostic::Diagnostics& diag) override;

        virtual void typeCheck(diagnostic::Diagnostics& diag, bool& exit) override;
        virtual bool triviallyImplicitCast(diagnostic::Diagnostics& diag, Type* destType) override;

        std::string getName();

    private:
        std::string mName;
    };
    using VariableExpressionPtr = std::unique_ptr<VariableExpression>;
}

#endif // VIPER_FRAMEWORK_PARSER_AST_EXPRESSION_VARIABLE_EXPRESSION_H