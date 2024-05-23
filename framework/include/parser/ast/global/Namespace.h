// Copyright 2024 solar-mist

#ifndef VIPER_FRAMEWORK_PARSER_AST_GLOBAL_NAMESPACE_H
#define VIPER_FRAMEWORK_PARSER_AST_GLOBAL_NAMESPACE_H 1

#include "parser/ast/Node.h"

namespace parser
{
    class Namespace : public ASTNode
    {
    public:
        Namespace(std::string_view name, std::vector<ASTNodePtr>&& body, Scope* scope);

        void typeCheck(Scope* scope, diagnostic::Diagnostics& diag) override;
        vipir::Value* emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope, diagnostic::Diagnostics& diag) override;

    private:
        std::string mName;
        std::vector<ASTNodePtr> mBody;
        ScopePtr mScope;
    };
    using NamespacePtr = std::unique_ptr<Namespace>;
}

#endif // VIPER_FRAMEWORK_PARSER_AST_GLOBAL_NAMESPACE_H
