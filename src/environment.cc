#include <environment.hh>
#include <diagnostics.hh>
#include <algorithm>

Environment::Environment(Environment* outer)
    :_outer(outer)
{
}

Environment::~Environment()
{
    for(VarSymbol* symbol : _varSymbols)
        delete symbol;
}

Environment* Environment::GetOuter() const
{
    return _outer;
}

std::map<std::string, SSA::AllocaInst*>& Environment::GetNamedValues()
{
    return _namedValues;
}

std::vector<VarSymbol*>& Environment::GetVarSymbols()
{
    return _varSymbols;
}

SSA::AllocaInst* Environment::FindNamedValue(const std::string& name)
{
    Environment* env = this;
    while(true)
    {
        if(env->GetNamedValues().find(name) != env->GetNamedValues().end())
            return env->GetNamedValues()[name];
        else if(env->GetOuter())
            env = env->GetOuter();
        else
            Diagnostics::Error("viper", "Unknown variable symbol: " + name + ".");
    }
}

VarSymbol* Environment::FindVarSymbol(const std::string& name)
{
    Environment* env = this;
    while(true)
    {
        std::vector<VarSymbol*> varSymbols = env->GetVarSymbols();
        auto res = std::find_if(varSymbols.begin(), varSymbols.end(), [&name](VarSymbol* var){
            return var->GetName() == name;
        });
        if(res != varSymbols.end())
            return *res;
        else if(env->GetOuter())
            env = env->GetOuter();
        else
            Diagnostics::Error("viper", "Unknown variable symbol: " + name + ".");
    }
}