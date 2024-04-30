// Copyright 2024 solar-mist

#ifndef VIPER_FRAMEWORK_PARSER_AST_GLOBAL_GLOBAL_DECLARATION_H
#define VIPER_FRAMEWORK_PARSER_AST_GLOBAL_GLOBAL_DECLARATION_H 1

#include "parser/ast/Node.h"

namespace parser
{
    class GlobalDeclaration : public ASTNode
    {
    public:
        GlobalDeclaration(std::string name, Type* type, ASTNodePtr initVal);

        vipir::Value* emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope) override;

    private:
        std::string mName;
        ASTNodePtr mInitVal;
    };
    using GlobalDeclarationPtr = std::unique_ptr<GlobalDeclaration>;
}

#endif // VIPER_FRAMEWORK_PARSER_AST_GLOBAL_STRUCT_DECLARATION_H