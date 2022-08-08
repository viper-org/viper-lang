#include <ast/var_decl.hh>
#include <globals.hh>
#include <iostream>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Instructions.h>
#include <llvm/Support/raw_ostream.h>

llvm::AllocaInst* create_alloca(llvm::Function* func, llvm::StringRef name, std::shared_ptr<quark_type> T, llvm::Value* array_size)
{
    llvm::IRBuilder<> tmp_builder(&func->getEntryBlock(), func->getEntryBlock().begin());
    return tmp_builder.CreateAlloca(T->get_type(), array_size, name);
}

var_decl::var_decl(std::shared_ptr<quark_type> type, std::string name, std::unique_ptr<ast_expr> value)
    :name(name), value(std::move(value))
{
    this->type = type;
}

void var_decl::print(std::ostream& stream) const
{
    stream << "<variable-decl>:\ntype:" << type->get_name() << "\nvalue:";
    value->print(stream);
}

expr_type var_decl::get_type() const
{
    return expr_type::VARIABLE_DECL;
}

llvm::Value* var_decl::codegen(std::shared_ptr<scope> env) const
{
    llvm::Function* func = builder.GetInsertBlock()->getParent();
    llvm::Value* init_val;

    if(value)
    {
        value->type = type;
        init_val = value->codegen(env);
    }

    llvm::AllocaInst* alloca = create_alloca(func, name, type);
    env->named_values[name] = alloca;

    if(value)
    {
        if(init_val->getType() != type->get_type())
                init_val = quark_type::convert(init_val, type->get_type());

        builder.CreateStore(init_val, alloca);

        return init_val; 
    }
    return nullptr;
}