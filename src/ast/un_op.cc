#include "globals.hh"
#include <ast/un_op.hh>
#include <ast/var.hh>
#include <llvm/IR/Constants.h>

un_op_expr::un_op_expr(token operand, std::unique_ptr<ast_expr> value)
    :value(std::move(value))
{
    switch(operand.type)
    {
        case token_type::increment:
            this->operand = unary_operand::INCREMENT;
            break;
        case token_type::decrement:
            this->operand = unary_operand::DECREMENT;
            break;
        default:
            break;
    }
}

std::string un_op_expr::operand_to_string() const
{
    switch(operand)
    {
        case unary_operand::INCREMENT:
            return "INCREMENT";
        case unary_operand::DECREMENT:
            return "DECREMENT";
        default:
            break;
    }
    return "";
}

void un_op_expr::print(std::ostream& stream) const
{
    stream << "<binary-operator>:\noperand: " << operand_to_string() << "\nvalue: ";
    value->print(stream);
}

expr_type un_op_expr::get_type() const
{
    return expr_type::UN_OP;
}

llvm::Value* un_op_expr::codegen(std::shared_ptr<scope> env) const
{
    if(operand == unary_operand::INCREMENT || operand == unary_operand::DECREMENT)
    {
        var_expr* var = static_cast<var_expr*>(value.get());

        llvm::Value* value;

        llvm::AllocaInst* alloca = find_named_value(var->get_name(), env);

        llvm::LoadInst* load = builder.CreateLoad(alloca->getAllocatedType(), alloca, var->get_name().data());

        if(operand == unary_operand::INCREMENT)
            value = builder.CreateAdd(load, llvm::ConstantInt::get(var->type.llvm_getter(ctx), 1), "inctmp");
        else
            value = builder.CreateAdd(load, llvm::ConstantInt::get(var->type.llvm_getter(ctx), -1), "dectmp");

        return builder.CreateStore(value, alloca);
    }
    
    return nullptr;
}