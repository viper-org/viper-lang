// Copyright 2024 solar-mist

#ifndef VIPER_FRAMEWORK_SYMBOL_SCOPE_H
#define VIPER_FRAMEWORK_SYMBOL_SCOPE_H 1

#include <memory>
#include <string>
#include <vector>

struct Scope
{
    Scope(Scope* parent, std::string namespaceName, bool isGlobalScope);

    std::vector<std::string> getNamespaces();

    Scope* parent;
    std::string namespaceName;
    bool isGlobalScope;
};
using ScopePtr = std::unique_ptr<Scope>;

#endif // VIPER_FRAMEWORK_SYMBOL_SCOPE_H