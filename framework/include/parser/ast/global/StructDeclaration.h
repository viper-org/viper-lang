// Copyright 2024 solar-mist

#ifndef VIPER_FRAMEWORK_PARSER_AST_GLOBAL_STRUCT_DECLARATION_H
#define VIPER_FRAMEWORK_PARSER_AST_GLOBAL_STRUCT_DECLARATION_H 1

#include "parser/ast/Node.h"

namespace parser
{
    struct StructField
    {
        std::string name;
        Type* type;
    };

    class StructDeclaration : public ASTNode
    {
    public:
        StructDeclaration(std::string name, std::vector<StructField> fields);

        vipir::Value* emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope) override;

    private:
        std::string mName;
        std::vector<StructField> mFields;
    };
    using StructDeclarationPtr = std::unique_ptr<StructDeclaration>;
}

#endif // VIPER_FRAMEWORK_PARSER_AST_GLOBAL_STRUCT_DECLARATION_H