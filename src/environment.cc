#include <environment.hh>
#include <diagnostics.hh>
#include <algorithm>
#include <iostream>

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

struct FunctionSignature
{
    FunctionSignature(std::string name,
    std::vector<std::shared_ptr<Type>> params,
    std::shared_ptr<Type> returnType) : name(name), params(params), returnType(returnType) {}
    std::string name;
    std::vector<std::shared_ptr<Type>> params;
    std::shared_ptr<Type> returnType;
};

std::vector<FunctionSignature> mangledFunctions;

std::string MangleFunction(std::string name, std::vector<std::shared_ptr<Type>> params, std::shared_ptr<Type> returnType)
{
    if(name == "_start" || name == "Main")
    {
        mangledFunctions.push_back({name, params, returnType});
        return name;
    }
    std::string res = "_Z";
    res += std::to_string(name.length());
    res += name;

    res += std::to_string(params.size());
    for(std::shared_ptr<Type> param : params)
        res += param->GetMangleID();
    
    res += "E";
    res += returnType->GetMangleID();

    mangledFunctions.push_back({name, params, returnType});

    return res;
}

std::string MangleFunction(FunctionSignature func)
{
    std::string res = "_Z";
    res += std::to_string(func.name.length());
    res += func.name;

    res += std::to_string(func.params.size());
    for(std::shared_ptr<Type> param : func.params)
        res += param->GetMangleID();
    
    res += "E";
    res += func.returnType->GetMangleID();

    return res;
}

std::string GetMangledFunction(std::string name, std::vector<std::shared_ptr<Type>> params)
{
    if(name == "Main" || name == "_start")
        return name;
    for(FunctionSignature func : mangledFunctions)
    {
        if(func.name == name && func.params.size() == params.size())
            return MangleFunction(func);
    }
    return "ERROR";
}