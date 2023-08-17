// Copyright 2023 solar-mist


#include <unordered_map>
#ifndef VIPER_FRAMEWORK_ENVIRONMENT_ENVIRONMENT_H
#define VIPER_FRAMEWORK_ENVIRONMENT_ENVIRONMENT_H 1

#include <vipir/IR/Instruction/AllocaInst.h>

extern std::unordered_map<std::string, vipir::AllocaInst*> variables; // TODO: Proper scopes/environments

#endif