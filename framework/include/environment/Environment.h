// Copyright 2023 solar-mist


#ifndef VIPER_FRAMEWORK_ENVIRONMENT_ENVIRONMENT_H
#define VIPER_FRAMEWORK_ENVIRONMENT_ENVIRONMENT_H 1

#include <vipir/IR/Instruction/AllocaInst.h>

#include <unordered_map>

extern std::unordered_map<std::string, vipir::AllocaInst*> variables; // TODO: Proper scopes/environments
extern std::unordered_map<std::string, vipir::Function*> functions; // TODO: Proper symbol table

#endif