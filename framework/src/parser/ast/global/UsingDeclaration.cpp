// Copyright 2024 solar-mist


#include "parser/ast/global/UsingDeclaration.h"

namespace parser
{
    UsingDeclaration::UsingDeclaration(std::string_view name, Type* type)
        : mName(name)
        , mType(type)
    {
        Type::AddAlias(mName, mType);
    }

    vipir::Value* UsingDeclaration::emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope, diagnostic::Diagnostics& diag)
    {
        return nullptr;
    }
}