#ifndef VIPER_ENVIRONMENT_HH
#define VIPER_ENVIRONMENT_HH
#include <ssa/value/instruction/alloca.hh>
#include <map>

extern std::map<std::string, SSA::AllocaInst*> namedValues;

#endif