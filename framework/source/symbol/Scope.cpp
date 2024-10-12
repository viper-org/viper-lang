// Copyright 2024 solar-mist

#include "symbol/Scope.h"

Scope::Scope(Scope* parent, std::string namespaceName, bool isGlobalScope)
    : parent(parent)
    , namespaceName(std::move(namespaceName))
    , isGlobalScope(isGlobalScope)
{
}

std::vector<std::string> Scope::getNamespaces()
{
    std::vector<std::string> namespaces;
    Scope* current = this;
    while (current)
    {
        namespaces.push_back(current->namespaceName);
        current = current->parent;
    }
    return namespaces;
}