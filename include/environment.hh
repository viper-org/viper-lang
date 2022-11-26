#ifndef VIPER_ENVIRONMENT_HH
#define VIPER_ENVIRONMENT_HH
#include <llvm/IR/Instructions.h>
#include <symbol/symbols.hh>
#include <unordered_map>

struct Environment
{
    Environment(std::shared_ptr<Environment> outer);
    
    std::shared_ptr<Environment> GetOuter() const;
    std::unordered_map<std::string, llvm::AllocaInst*>& GetNamedValues();
    std::vector<std::shared_ptr<VarSymbol>>& GetVarSymbols();

    llvm::AllocaInst* FindNamedValue(const std::string& name);
    std::shared_ptr<VarSymbol> FindVarSymbol(const std::string& name);
private:
    std::shared_ptr<Environment> _outer;
    std::unordered_map<std::string, llvm::AllocaInst*> _namedValues;
    std::vector<std::shared_ptr<VarSymbol>> _varSymbols;
};

std::string MangleFunction(std::string_view name, std::vector<std::shared_ptr<Type>> params, std::shared_ptr<Type> returnType);

#endif