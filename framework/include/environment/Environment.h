// Copyright 2023 solar-mist


#ifndef VIPER_FRAMEWORK_ENVIRONMENT_ENVIRONMENT_H
#define VIPER_FRAMEWORK_ENVIRONMENT_ENVIRONMENT_H 1

#include "type/Type.h"

#include "symbol/Symbol.h"

#include <vipir/IR/Instruction/AllocaInst.h>

#include <optional>
#include <unordered_map>

struct LocalSymbol
{
    LocalSymbol() = default;
    LocalSymbol(vipir::AllocaInst* alloca, Type* type);

    vipir::AllocaInst* alloca;
    Type* type;
};

struct Environment
{
    std::unordered_map<std::string, LocalSymbol> variables; // For codegen
    SymbolTable symbols; // For use in parser

    Environment* parent;

    LocalSymbol findVariable(const std::string& name);
    std::optional<Symbol> findSymbol(const std::string& name);
};
using EnvironmentPtr = std::unique_ptr<Environment>;

extern std::unordered_map<std::string, vipir::Function*> functions; // TODO: Proper symbol table

#endif