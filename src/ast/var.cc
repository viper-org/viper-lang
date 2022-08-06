#include <ast/var.hh>
#include <globals.hh>
#include <llvm/IR/Instructions.h>

var_expr::var_expr(std::string name)
    :name(name)
{
}

void var_expr::print(std::ostream& stream) const
{
    stream << "<variable-expression>:\nname: " << name;
}

expr_type var_expr::get_type() const
{
    return expr_type::VARIABLE;
}

llvm::Value* var_expr::codegen(std::shared_ptr<scope> env) const
{
    llvm::AllocaInst* alloca = find_named_value(name, env);

    return builder.CreateLoad(alloca->getAllocatedType(), alloca, name.data());
}