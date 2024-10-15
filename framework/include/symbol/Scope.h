// Copyright 2024 solar-mist

#ifndef VIPER_FRAMEWORK_SYMBOL_SCOPE_H
#define VIPER_FRAMEWORK_SYMBOL_SCOPE_H 1

#include "type/Type.h"

#include <vipir/IR/Value.h>

#include <memory>
#include <string>
#include <vector>

struct Symbol
{
    Symbol(std::string name, Type* type);

    std::string name;
    Type* type;
    vipir::Value* value;
    unsigned long id;
};

struct Scope
{
    Scope(Scope* parent, std::string namespaceName, bool isGlobalScope, Type* currentReturnType = nullptr);

    std::vector<std::string> getNamespaces();

    Symbol* getSymbol(unsigned long id);
    Symbol* resolveSymbol(std::string name);
    std::vector<Symbol*> getCandidateFunctions(std::string name);

    Scope* parent;

    std::string namespaceName;
    bool isGlobalScope;

    Type* currentReturnType;
    std::vector<Symbol> symbols;
};
using ScopePtr = std::unique_ptr<Scope>;

#endif // VIPER_FRAMEWORK_SYMBOL_SCOPE_H