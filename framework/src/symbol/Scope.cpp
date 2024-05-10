// Copyright 2024 solar-mist


#include "symbol/Scope.h"

#include <iostream>

std::unordered_map<std::string, FunctionSymbol> GlobalFunctions;
std::unordered_map<std::string, GlobalSymbol>   GlobalVariables;

LocalSymbol::LocalSymbol(vipir::AllocaInst* alloca, Type* type)
    : alloca{alloca}
    , type(type)
{
}

FunctionSymbol::FunctionSymbol(vipir::Function* function, bool priv)
    : function(function)
    , priv(priv)
{
}

GlobalSymbol::GlobalSymbol(vipir::GlobalVar* global)
    : global(global)
{
}

Scope::Scope(Scope* parent, StructType* owner)
    : parent(parent)
    , owner(owner)
{
}

LocalSymbol* Scope::findVariable(const std::string& name)
{
    Scope* scope = this;
    while (scope)
    {
        if (scope->locals.find(name) != scope->locals.end())
        {
            return &scope->locals.at(name);
        }

        scope = scope->parent;
    }

    return nullptr;
}