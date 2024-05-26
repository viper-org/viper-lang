// Copyright 2024 solar-mist


#include "parser/ast/global/UsingDeclaration.h"

#include <utility>

namespace parser
{
    UsingDeclaration::UsingDeclaration(std::vector<std::string> names, Type* type)
        : mNames(std::move(names))
        , mType(type)
    {
        Type::AddAlias(mNames, mType);
    }

    void UsingDeclaration::typeCheck(Scope* scope, diagnostic::Diagnostics& diag)
    {
    }

    vipir::Value* UsingDeclaration::emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope, diagnostic::Diagnostics& diag)
    {
        return nullptr;
    }
}