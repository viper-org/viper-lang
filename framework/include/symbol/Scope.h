// Copyright 2024 solar-mist


#ifndef VIPER_FRAMEWORK_SCOPE_SCOPE_H
#define VIPER_FRAMEWORK_SCOPE_SCOPE_H 1

#include "type/StructType.h"

#include <vipir/IR/Instruction/AllocaInst.h>
#include <vipir/IR/Function.h>
#include <vipir/IR/GlobalVar.h>

#include <optional>
#include <unordered_map>

struct LocalSymbol
{
    LocalSymbol() = default;
    LocalSymbol(vipir::AllocaInst* alloca);

    vipir::AllocaInst* alloca;
};

struct FunctionSymbol
{
    FunctionSymbol() = default;
    FunctionSymbol(vipir::Function* function, bool priv);

    vipir::Function* function;
    bool priv;
};
struct GlobalSymbol
{
    GlobalSymbol() = default;
    GlobalSymbol(vipir::GlobalVar* global);

    vipir::GlobalVar* global;
};
extern std::unordered_map<std::string, FunctionSymbol> GlobalFunctions;
extern std::unordered_map<std::string, GlobalSymbol> GlobalVariables;

struct Scope
{
    Scope(Scope* parent, StructType* owner);

    std::unordered_map<std::string, LocalSymbol> locals;

    LocalSymbol* findVariable(const std::string& name);

    Scope* parent;
    StructType* owner;
};
using ScopePtr = std::unique_ptr<Scope>;

#endif // VIPER_FRAMEWORK_SCOPE_SCOPE_H