#include <environment.hxx>
#include <diagnostics.hxx>

namespace Viper
{
    llvm::AllocaInst* FindNamedValue(std::string name, std::shared_ptr<Environment> scope)
    {
        while(true)
        {
            if(scope->namedValues.find(name) != scope->namedValues.end())
                return scope->namedValues[name];
            else if(scope->outer)
                scope = scope->outer;
        }
    }

    llvm::BasicBlock* FindLabel(std::string name, std::shared_ptr<Environment> scope)
    {
        while(true)
        {
            for(llvm::BasicBlock* block : scope->labels)
            {
                if(block->getName() == name)
                    return block;
            }
            if(scope->outer)
                scope = scope->outer;
            else
                throw; // TODO: Error properly
        }
    }
}