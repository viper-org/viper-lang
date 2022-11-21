#ifndef VIPER_ENVIRONMENT_HH
#define VIPER_ENVIRONMENT_HH
#include <llvm/IR/Instructions.h>
#include <string>
#include <unordered_map>

struct Environment
{
    Environment(std::shared_ptr<Environment> outer);
    
    std::shared_ptr<Environment> GetOuter() const;
    std::unordered_map<std::string, llvm::AllocaInst*>& GetNamedValues();

    llvm::AllocaInst* FindNamedValue(const std::string& name);
private:
    std::shared_ptr<Environment> _outer;
    std::unordered_map<std::string, llvm::AllocaInst*> _namedValues;
};

#endif