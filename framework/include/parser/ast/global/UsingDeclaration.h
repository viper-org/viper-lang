// Copyright 2024 solar-mist

#ifndef VIPER_FRAMEWORK_PARSER_AST_USING_DECLARATION_H
#define VIPER_FRAMEWORK_PARSER_AST_USING_DECLARATION_H 1

#include "parser/ast/Node.h"

namespace parser
{
    class UsingDeclaration : public ASTNode
    {
    public:
        UsingDeclaration(std::vector<std::string> names, Type* type);

        void typeCheck(Scope* scope, diagnostic::Diagnostics& diag) override;
        vipir::Value* emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope, diagnostic::Diagnostics& diag) override;

    private:
        std::vector<std::string> mNames;
        Type* mType;
    };
    using UsingDeclarationPtr = std::unique_ptr<UsingDeclaration>;
}

#endif // VIPER_FRAMEWORK_PARSER_AST_USING_DECLARATION_H