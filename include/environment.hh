#ifndef VIPER_ENVIRONMENT_HH
#define VIPER_ENVIRONMENT_HH
#include <llvm/IR/Instructions.h>
#include <string>
#include <unordered_map>

extern std::unordered_map<std::string, llvm::AllocaInst*> namedValues;

#endif