#include "ast/var_decl.hh"
#include <ast/function.hh>
#include <iostream>
#include <globals.hh>
#include <llvm/IR/Argument.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Instructions.h>

function_ast::function_ast(std::string name, type_info type, std::vector<std::unique_ptr<ast_expr>> body, std::vector<std::pair<type_info, std::string>> args, std::shared_ptr<scope> env)
    :name(name), type(type), body(std::move(body)), args(args), env(env)
{
}

llvm::Function* function_ast::codegen()
{
    std::vector<llvm::Type*> arg_types;
    for(std::pair<type_info, std::string> arg : args)
        arg_types.push_back(arg.first.llvm_info.getter(ctx));
    
    llvm::FunctionType* func_type = llvm::FunctionType::get(type.llvm_info.getter(ctx), arg_types, false);
    llvm::Function* func = llvm::Function::Create(func_type, llvm::Function::ExternalLinkage, std::string(name.data()), module);

    unsigned int i = 0;
    for(llvm::Argument& arg : func->args())
        arg.setName(args[i++].second);

    llvm::BasicBlock* bb = llvm::BasicBlock::Create(ctx, std::string(name.data()), func);
    builder.SetInsertPoint(bb);

    env->named_values.clear();
    i = 0;
    for(llvm::Argument& arg : func->args())
    {
        llvm::AllocaInst* alloca = create_alloca(func, arg.getName(), args[i].first);
        builder.CreateStore(&arg, alloca);
        env->named_values[std::string(arg.getName())] = alloca;
        i++;
    }
    for(std::unique_ptr<ast_expr>& expr : body)
    {
        if(expr->get_type() == expr_type::RETURN)
        {
            expr->codegen(env);
            return func;
        }
        else
            expr->codegen(env);
    }
    builder.CreateRet(llvm::ConstantInt::get(type.llvm_info.getter(ctx), 0));
    return func;
}