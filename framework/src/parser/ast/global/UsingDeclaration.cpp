// Copyright 2024 solar-mist


#include "parser/ast/global/UsingDeclaration.h"

namespace parser
{
    UsingDeclaration::UsingDeclaration(std::vector<std::string> names, Type* type)
        : mNames(names)
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