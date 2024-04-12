// Copyright 2024 solar-mist

#ifndef VIPER_FRAMEWORK_PARSER_AST_STATEMENT_VARIABLE_DECLARATION_H
#define VIPER_FRAMEWORK_PARSER_AST_STATEMENT_VARIABLE_DECLARATION_H 1

#include "parser/ast/Node.h"

namespace parser
{
    class VariableDeclaration : public ASTNode
    {
    public:
        VariableDeclaration(std::string&& name, ASTNodePtr&& initialValue);

        vipir::Value* emit(vipir::IRBuilder& builder, vipir::Module& module) override;

    private:
        std::string mName;
        ASTNodePtr mInitialValue;
    };
    using VariableDeclarationPtr = std::unique_ptr<VariableDeclaration>;
}

#endif // VIPER_FRAMEWORK_PARSER_AST_STATEMENT_VARIABLE_DECLARATION_H