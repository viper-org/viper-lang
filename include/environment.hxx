#ifndef VIPER_ENVIRONMENT_HXX
#define VIPER_ENVIRONMENT_HXX
#include <map>
#include <memory>
#include <string>
#include <llvm/IR/Instructions.h>

namespace Viper
{
    struct Environment
    {
        std::map<std::string, llvm::AllocaInst*> namedValues;

        std::shared_ptr<Environment> outer;
    };

    llvm::AllocaInst* FindNamedValue(std::string name, std::shared_ptr<Environment> scope);
}

#endif