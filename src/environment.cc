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
            Diagnostics::Error("viper", "Unknown identifier: " + name + ".");
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
    FunctionSignature(std::vector<std::string> identifiers,
    std::vector<std::shared_ptr<Type>> params,
    std::shared_ptr<Type> returnType) : identifiers(identifiers), params(params), returnType(returnType) {}
    std::vector<std::string> identifiers;
    std::vector<std::shared_ptr<Type>> params;
    std::shared_ptr<Type> returnType;
};

std::vector<FunctionSignature> mangledFunctions;

std::string MangleFunction(std::vector<std::string> identifiers, std::vector<std::shared_ptr<Type>> params, std::shared_ptr<Type> returnType)
{
    if(identifiers[0] == "_start" || identifiers[0] == "Main")
    {
        mangledFunctions.push_back({identifiers, params, returnType});
        return identifiers[0];
    }
    std::string res = "_Z";

    std::string name;
    for(std::string_view ident : identifiers)
    {
        name += std::to_string(ident.length());
        name += ident;
    }
    res += name;

    res += std::to_string(params.size());
    for(std::shared_ptr<Type> param : params)
        res += param->GetMangleID();
    
    res += "E";
    res += returnType->GetMangleID();

    mangledFunctions.push_back({identifiers, params, returnType});

    return res;
}

std::string MangleFunction(FunctionSignature func)
{
    std::string res = "_Z";
    for(std::string_view ident : func.identifiers)
    {
        res += std::to_string(ident.length());
        res += ident;
    }

    res += std::to_string(func.params.size());
    for(std::shared_ptr<Type> param : func.params)
        res += param->GetMangleID();
    
    res += "E";
    res += func.returnType->GetMangleID();

    return res;
}

std::string GetMangledFunction(std::vector<std::string> identifiers, std::vector<std::shared_ptr<Type>> params)
{
    if(identifiers[0] == "Main" || identifiers[0] == "_start")
        return identifiers[0];
    std::string name;
    for(std::string_view ident : identifiers)
    {
        name += std::to_string(ident.length());
        name += ident;
    }
    
    for(FunctionSignature func : mangledFunctions)
    {
        if(func.identifiers == identifiers && func.params.size() == params.size())
        {
            bool found = true;
            for(size_t i = 0; i < params.size(); i++)
            {
                if(func.params[i]->GetLLVMType() != params[i]->GetLLVMType())
                {
                    found = false;
                    break;
                }
            }
            if(found)
                return MangleFunction(func);
        }
    }
    return "ERROR";
}