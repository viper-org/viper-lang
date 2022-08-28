#ifndef VIPER_GLOBALS_HH
#define VIPER_GLOBALS_HH
#include <codegen/value/instruction/alloca.hh>
#include <map>

extern std::map<std::string, Codegen::AllocaInst*> namedValues;

#endif