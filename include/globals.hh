#ifndef QRK_GLOBALS_HH
#define QRK_GLOBALS_HH
#include <llvm/ADT/StringRef.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/Support/raw_ostream.h>
#include <memory>
#include <map>
#include <optional>

struct scope
{
    std::map<std::string, llvm::AllocaInst*> named_values;
    std::vector<llvm::BasicBlock*> labels;

    std::shared_ptr<scope> outer;
};

inline llvm::AllocaInst*& find_named_value(std::string name, std::shared_ptr<scope> env)
{
    while(1)
    {
        if(env->named_values.find(name) != env->named_values.end())
            return env->named_values[name];
        else if(env->outer)
            env = env->outer;
    }
}

inline llvm::BasicBlock* find_label(std::string name, std::shared_ptr<scope> env)
{
    while(1)
    {
        for(llvm::BasicBlock* bb : env->labels)
        {
            if(bb->getName() == name)
                return bb;
        }
        if(env->outer)
            env = env->outer;
    }
}

extern llvm::LLVMContext                   ctx;
extern llvm::IRBuilder<>                   builder;
extern llvm::Module                        module;

#endif