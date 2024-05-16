// Copyright 2024 solar-mist

#ifndef VIPER_FRAMEWORK_PARSER_AST_GLOBAL_GLOBAL_DECLARATION_H
#define VIPER_FRAMEWORK_PARSER_AST_GLOBAL_GLOBAL_DECLARATION_H 1

#include "parser/ast/Node.h"

namespace parser
{
    class GlobalDeclaration : public ASTNode
    {
    public:
        GlobalDeclaration(std::vector<std::string> names, Type* type, ASTNodePtr initVal);

        vipir::Value* emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope, diagnostic::Diagnostics& diag) override;

    private:
        std::vector<std::string> mNames;
        ASTNodePtr mInitVal;
    };
    using GlobalDeclarationPtr = std::unique_ptr<GlobalDeclaration>;
}

#endif // VIPER_FRAMEWORK_PARSER_AST_GLOBAL_STRUCT_DECLARATION_H