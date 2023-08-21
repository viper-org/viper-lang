// Copyright 2023 solar-mist


#ifndef VIPER_FRAMEWORK_PARSER_AST_EXPRESSION_VARIABLE_H
#define VIPER_FRAMEWORK_PARSER_AST_EXPRESSION_VARIABLE_H 1

#include "parser/ast/Node.h"

namespace parsing
{
    class Variable : public ASTNode
    {
    public:
        Variable(const std::string& name);

        vipir::Value* emit(vipir::Builder& builder, vipir::Module& module, Environment* scope) override;

    private:
        std::string mName;
    };

    using VariablePtr = std::unique_ptr<Variable>;
}

#endif