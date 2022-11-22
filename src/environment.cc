#include <environment.hh>
#include <diagnostics.hh>
#include <algorithm>

Environment::Environment(std::shared_ptr<Environment> outer)
    :_outer(outer)
{
}

std::shared_ptr<Environment> Environment::GetOuter() const
{
    return _outer;
}

std::unordered_map<std::string, llvm::AllocaInst*>& Environment::GetNamedValues()
{
    return _namedValues;
}

std::vector<std::shared_ptr<VarSymbol>>& Environment::GetVarSymbols()
{
    return _varSymbols;
}


llvm::AllocaInst* Environment::FindNamedValue(const std::string& name)
{
    Environment* env = this;
    while(true)
    {
        if(env->GetNamedValues().find(name) != env->GetNamedValues().end())
            return env->GetNamedValues()[name];
        else if(env->GetOuter())
            env = env->GetOuter().get();
        else
            Diagnostics::Error("viper", "Unknown variable symbol: " + name + ".");
    }
}

std::shared_ptr<VarSymbol> Environment::FindVarSymbol(const std::string& name)
{
    Environment* env = this;
    while(true)
    {
        std::vector<std::shared_ptr<VarSymbol>> varSymbols = env->GetVarSymbols();
        auto res = std::find_if(varSymbols.begin(), varSymbols.end(), [&name](std::shared_ptr<VarSymbol> var){
            return var->GetName() == name;
        });
        if(res != varSymbols.end())
            return *res;
        else if(env->GetOuter())
            env = env->GetOuter().get();
        else
            Diagnostics::Error("viper", "Unknown variable symbol: " + name + ".");
    }
}