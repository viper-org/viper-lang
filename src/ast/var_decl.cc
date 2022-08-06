#include <ast/var_decl.hh>
#include <globals.hh>
#include <iostream>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Instructions.h>
#include <llvm/Support/raw_ostream.h>

llvm::AllocaInst* create_alloca(llvm::Function* func, llvm::StringRef name, type_info type)
{
    llvm::IRBuilder<> tmp_builder(&func->getEntryBlock(), func->getEntryBlock().begin());
    return tmp_builder.CreateAlloca(type.llvm_info.getter(ctx), nullptr, name);
}

var_decl::var_decl(type_info type, std::string name, std::unique_ptr<ast_expr> value)
    :name(name), value(std::move(value))
{
    this->type = type;
}

void var_decl::print(std::ostream& stream) const
{
    stream << "<variable-decl>:\ntype:" << type.name << "\nvalue:";
    value->print(stream);
}

expr_type var_decl::get_type() const
{
    return expr_type::VARIABLE_DECL;
}

llvm::Value* var_decl::codegen(std::shared_ptr<scope> env) const
{
    llvm::Function* func = builder.GetInsertBlock()->getParent();

    value->type = type;
    llvm::Value* init_val = value->codegen(env);

    llvm::AllocaInst* alloca = create_alloca(func, name, type);

    if(init_val->getType() != type.llvm_info.getter(ctx))
            init_val = type_info::convert(init_val, type.llvm_info.getter(ctx));

    builder.CreateStore(init_val, alloca);

    env->named_values[name] = alloca;

    return init_val;   
}