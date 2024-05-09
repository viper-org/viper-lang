// Copyright 2024 solar-mist

#ifndef VIPER_FRAMEWORK_PARSER_AST_GLOBAL_STRUCT_DECLARATION_H
#define VIPER_FRAMEWORK_PARSER_AST_GLOBAL_STRUCT_DECLARATION_H 1

#include "parser/ast/Node.h"
#include "parser/ast/global/Function.h"

namespace parser
{
    struct StructField
    {
        bool priv;
        std::string name;
        Type* type;
    };

    struct StructMethod
    {
        bool priv;
        std::string name;
        Type* returnType;
        std::vector<FunctionArgument> arguments;
        std::vector<ASTNodePtr> body;
        ScopePtr scope;
    };

    class StructDeclaration : public ASTNode
    {
    public:
        StructDeclaration(std::string name, std::vector<StructField> fields, std::vector<StructMethod> methods);

        vipir::Value* emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope) override;

    private:
        std::string mName;
        std::vector<StructField> mFields;
        std::vector<StructMethod> mMethods;
    };
    using StructDeclarationPtr = std::unique_ptr<StructDeclaration>;
}

#endif // VIPER_FRAMEWORK_PARSER_AST_GLOBAL_STRUCT_DECLARATION_H
