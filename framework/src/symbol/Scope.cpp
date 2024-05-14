// Copyright 2024 solar-mist


#include "symbol/Scope.h"
#include "symbol/NameMangling.h"

#include <algorithm>

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

FunctionSymbol* FindFunction(std::vector<std::string> names, std::vector<Type*> arguments)
{
    auto last = std::unique(names.begin(), names.end());
    names.erase(last, names.end());
    for (auto& [name, func] : GlobalFunctions)
    {
        auto funcNames = func.names;
        auto namesCopy = names;

        bool hasNames = true;
        for (auto name : funcNames)
        {
            if (std::find(namesCopy.begin(), namesCopy.end(), name) == namesCopy.end())
            {
                hasNames = false;
            }
        }
        if (!hasNames) continue;

        std::string mangledName = symbol::mangleFunctionName(names, arguments);
        if (mangledName == name) return &func;
    }
    return nullptr;
}

Scope::Scope(Scope* parent, StructType* owner)
    : parent(parent)
    , owner(owner)
    , breakTo(nullptr)
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