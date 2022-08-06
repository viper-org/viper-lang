#include <ast/extern_func.hh>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>

extern_func::extern_func(std::string name, std::shared_ptr<quark_type> type, std::vector<std::pair<std::shared_ptr<quark_type>, std::string>> args)
    :name(name), type(type), args(args)
{
}

llvm::Function* extern_func::codegen() const
{
    std::vector<llvm::Type*> arg_types;
    for(std::pair<std::shared_ptr<quark_type>, std::string> arg : args)
        arg_types.push_back(arg.first->get_type());
    
    llvm::FunctionType* func_type = llvm::FunctionType::get(type->get_type(), arg_types, false);

    llvm::Function* func = llvm::Function::Create(func_type, llvm::Function::ExternalLinkage, name, module);

    unsigned int i = 0;
    for(llvm::Argument& arg : func->args())
        arg.setName(args[i++].second);

    return func;
}