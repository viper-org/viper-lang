// Copyright 2024 solar-mist

#ifndef VIPER_FRAMEWORK_PARSER_AST_EXPRESSION_VARIABLE_EXPRESSION_H
#define VIPER_FRAMEWORK_PARSER_AST_EXPRESSION_VARIABLE_EXPRESSION_H 1

#include "parser/ast/Node.h"

#include "lexer/Token.h"

namespace parser
{
    class VariableExpression : public ASTNode
    {
    friend class CallExpression;
    public:
        VariableExpression(std::string&& name, Type* type, lexing::Token token);
        
        std::string getName();

        void typeCheck(Scope* scope, diagnostic::Diagnostics& diag) override;
        vipir::Value* emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope, diagnostic::Diagnostics& diag) override;

    private:
        std::string mName;
        lexing::Token mToken;
    };
    using VariableExpressionPtr = std::unique_ptr<VariableExpression>;
}

#endif // VIPER_FRAMEWORK_PARSER_AST_EXPRESSION_VARIABLE_EXPRESSION_H