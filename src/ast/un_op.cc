#include "token.hh"
#include <ast/un_op.hh>
#include <ast/var.hh>
#include <llvm/IR/Constants.h>
#include <iostream>

un_op_expr::un_op_expr(token operand, std::unique_ptr<ast_expr> value)
    :value(std::move(value))
{
    switch(operand.type)
    {
        case token_type::plus:
            this->operand = unary_operand::UNARY_PLUS;
            break;
        case token_type::minus:
            this->operand = unary_operand::NEGATION;
            break;
        case token_type::hash:
            this->operand = unary_operand::ADDRESSOF;
            break;
        case token_type::star:
            this->operand = unary_operand::DEREFERENCE;
            break;
        default:
            break;
    }
}

std::string un_op_expr::operand_to_string() const
{
    switch(operand)
    {
        case unary_operand::UNARY_PLUS:
            return "UNARY_PLUS";
        case unary_operand::NEGATION:
            return "NEGATION";
        case unary_operand::ADDRESSOF:
            return "ADDRESSOF";
        case unary_operand::DEREFERENCE:
            return "DEREFERENCE";
        default:
            break;
    }
    return "";
}

void un_op_expr::print(std::ostream& stream) const
{
    stream << "<unary-operator>:\noperator: " << operand_to_string() << "\noperand: ";
    value->print(stream);
}

expr_type un_op_expr::get_type() const
{
    return expr_type::UN_OP;
}

llvm::Value* un_op_expr::codegen(std::shared_ptr<scope> env) const
{

    llvm::Value* value_codegen = value->codegen(env);

    switch(operand)
    {
        case unary_operand::NEGATION:
            return builder.CreateNeg(value_codegen, "negtmp");
        case unary_operand::UNARY_PLUS:
            return value_codegen;
        case unary_operand::ADDRESSOF:
        {
            var_expr* var = static_cast<var_expr*>(value.get());
            return find_named_value(var->get_name(), env);
        }
        case unary_operand::DEREFERENCE:
        {
            var_expr* var = static_cast<var_expr*>(value.get());

            llvm::AllocaInst* alloca = find_named_value(var->get_name(), env);

            llvm::outs() << *alloca->getAllocatedType() << "\n" << *alloca->getType() << "\n";

            llvm::LoadInst* load = builder.CreateLoad(alloca->getAllocatedType(), alloca, "dereftmp");

            return builder.CreateLoad(alloca->getAllocatedType(), load, "dereftmp");
        }
        default:
            break;
    }
    
    return nullptr;
}