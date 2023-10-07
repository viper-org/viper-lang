// Copyright 2023 solar-mist


#include "environment/Environment.h"
#include "type/Type.h"
#include "vipir/IR/Instruction/AllocaInst.h"

#include <iostream>

LocalSymbol::LocalSymbol(vipir::AllocaInst* alloca, Type* type)
    : alloca{alloca}
    , type(type)
{
}

LocalSymbol Environment::findVariable(const std::string& name)
{
    Environment* environment = this;
    while (environment)
    {
        if (environment->variables.find(name) != environment->variables.end())
        {
            return environment->variables.at(name);
        }

        environment = environment->parent;
    }
    std::cerr << "Unknown local symbol: " << name << "\n";
    std::exit(1);
}

std::optional<Symbol> Environment::findSymbol(const std::string& name)
{
    Environment* environment = this;
    while (environment)
    {
        if (environment->symbols.find(name) != environment->symbols.end())
        {
            return environment->symbols.at(name);
        }

        environment = environment->parent;
    }
    
    return std::nullopt;
}

std::unordered_map<std::string, vipir::Function*> functions;