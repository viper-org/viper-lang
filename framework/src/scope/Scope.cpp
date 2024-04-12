// Copyright 2024 solar-mist


#include "scope/Scope.h"

#include <iostream>

LocalSymbol::LocalSymbol(vipir::AllocaInst* alloca)
    : alloca{alloca}
{
}

Scope::Scope(Scope* parent)
    : parent(parent)
{
}

LocalSymbol Scope::findVariable(const std::string& name)
{
    Scope* scope = this;
    while (scope)
    {
        if (scope->locals.find(name) != scope->locals.end())
        {
            return scope->locals.at(name);
        }

        scope = scope->parent;
    }

    std::cerr << "Unknown local symbol: " << name << "\n"; // TODO: Report error properly
    std::exit(1);
}