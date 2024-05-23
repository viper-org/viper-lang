// Copyright 2024 solar-mist


#include "parser/ast/global/Namespace.h"

namespace parser
{
    Namespace::Namespace(std::string_view name, std::vector<ASTNodePtr>&& body, Scope* scope)
        : mName(std::move(name))
        , mBody(std::move(body))
        , mScope(scope)
    {
    }

    void Namespace::typeCheck(Scope* scope, diagnostic::Diagnostics& diag)
    {
        for (auto& node : mBody)
        {
            node->typeCheck(scope, diag);
        }
    }

    vipir::Value* Namespace::emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope, diagnostic::Diagnostics& diag)
    {
        scope = mScope.get();
        scope->namespaceName = mName;

        for (auto& value : mBody)
        {
            value->emit(builder, module, scope, diag);
        }

        return nullptr;
    }
}