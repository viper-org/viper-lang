// Copyright 2024 solar-mist


#include "symbol/Scope.h"
#include "symbol/NameMangling.h"
#include "symbol/Identifier.h"

#include <algorithm>

std::unordered_map<std::string, FunctionSymbol> GlobalFunctions;
std::unordered_map<std::string, GlobalSymbol>   GlobalVariables;

LocalSymbol::LocalSymbol(vipir::AllocaInst* alloca, Type* type)
    : alloca{alloca}
    , type(type)
{
}

FunctionSymbol::FunctionSymbol(vipir::Function* function, Type* type, bool priv, bool mangle)
    : function(function)
    , priv(priv)
    , mangle(mangle)
    , type(static_cast<FunctionType*>(type))
{
}

void FunctionSymbol::Create(vipir::Function* function, std::string mangledName, std::vector<std::string> names, Type* type, bool priv, bool mangle)
{
    symbol::AddIdentifier(mangledName, names);

    GlobalFunctions[mangledName] = FunctionSymbol(function, type, priv, mangle);
    GlobalFunctions[mangledName].names = std::move(names);
}

GlobalSymbol::GlobalSymbol(vipir::Value* global, Type* type)
    : global(global)
    , type(type)
{
}

FunctionSymbol* FindFunction(std::vector<std::string> givenNames, std::vector<std::string> activeNames, std::vector<Type*> arguments)
{
    std::vector<std::string> mangledNames = symbol::GetSymbol(givenNames, activeNames);

    for (auto name : mangledNames)
    {
        if (GlobalFunctions.find(name) != GlobalFunctions.end())
        {
            return &GlobalFunctions.at(name);
        }
    }

    return nullptr;
}

Scope::Scope(Scope* parent, StructType* owner)
    : parent(parent)
    , owner(owner)
    , breakTo(nullptr)
    , continueTo(nullptr)
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

vipir::BasicBlock* Scope::findBreakBB()
{
    Scope* scope = this;
    while (scope)
    {
        if (scope->breakTo)
        {
            return scope->breakTo;
        }

        scope = scope->parent;
    }

    return nullptr;
}

vipir::BasicBlock* Scope::findContinueBB()
{
    Scope* scope = this;
    while (scope)
    {
        if (scope->continueTo)
        {
            return scope->continueTo;
        }

        scope = scope->parent;
    }

    return nullptr;
}

StructType* Scope::findOwner()
{
    Scope* scope = this;
    while (scope)
    {
        if (scope->owner)
        {
            return scope->owner;
        }

        scope = scope->parent;
    }

    return nullptr;
}

std::vector<std::string> Scope::getNamespaces()
{
    std::vector<std::string> ret;

    Scope* scope = this;
    while (scope)
    {
        if (!scope->namespaceName.empty())
        {
            ret.push_back(scope->namespaceName);
        }

        scope = scope->parent;
    }

    std::reverse(ret.begin(), ret.end());
    return ret;
}