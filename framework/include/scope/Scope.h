// Copyright 2024 solar-mist


#ifndef VIPER_FRAMEWORK_SCOPE_SCOPE_H
#define VIPER_FRAMEWORK_SCOPE_SCOPE_H 1

#include <vipir/IR/Instruction/AllocaInst.h>

#include <optional>
#include <unordered_map>

struct LocalSymbol
{
    LocalSymbol() = default;
    LocalSymbol(vipir::AllocaInst* alloca);

    vipir::AllocaInst* alloca;
};

struct Scope
{
    Scope(Scope* parent);

    std::unordered_map<std::string, LocalSymbol> locals;

    LocalSymbol findVariable(const std::string& name);

    Scope* parent;
};
using ScopePtr = std::unique_ptr<Scope>;

#endif // VIPER_FRAMEWORK_SCOPE_SCOPE_H