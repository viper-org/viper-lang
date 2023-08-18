// Copyright 2023 solar-mist


#ifndef VIPER_FRAMEWORK_PARSER_AST_STATEMENT_VARIABLE_DECLARATION_H
#define VIPER_FRAMEWORK_PARSER_AST_STATEMENT_VARIABLE_DECLARATION_H 1

#include "parser/ast/Node.h"

namespace parsing
{
    class VariableDeclaration : public ASTNode
    {
    public:
        VariableDeclaration(Type* type, std::string&& name, ASTNodePtr value);

        vipir::Value* emit(vipir::Builder& builder, vipir::Module& module) override;

    private:
        std::string mName;
        ASTNodePtr mValue;
    };

    using VariableDeclarationPtr = std::unique_ptr<VariableDeclaration>;
}

#endif