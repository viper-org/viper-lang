#ifndef VIPER_ENVIRONMENT_HH
#define VIPER_ENVIRONMENT_HH
#include <ssa/value/instruction/alloca.hh>
#include <symbol/symbols.hh>
#include <map>

struct Environment
{
    Environment(Environment* outer);
    ~Environment();
    
    Environment* GetOuter() const;
    std::map<std::string, SSA::AllocaInst*>& GetNamedValues();
    std::vector<VarSymbol*>& GetVarSymbols();

    SSA::AllocaInst* FindNamedValue(const std::string& name);
    VarSymbol* FindVarSymbol(const std::string& name);
private:
    Environment* _outer;
    std::map<std::string, SSA::AllocaInst*> _namedValues;
    std::vector<VarSymbol*> _varSymbols;
};

#endif