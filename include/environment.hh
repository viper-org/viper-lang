#ifndef VIPER_ENVIRONMENT_HH
#define VIPER_ENVIRONMENT_HH
#include <ssa/value/instruction/alloca.hh>
#include <symbol/symbols.hh>
#include <map>

extern std::map<std::string, SSA::AllocaInst*> namedValues;
extern std::vector<VarSymbol*> varSymbols;

#endif