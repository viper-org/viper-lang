#include "globals.hh"
#include "typing/type.hh"
#include <ast/subscript.hh>
#include <ast/var.hh>
#include <iostream>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Value.h>
#include <llvm/Support/raw_ostream.h>
#include <ostream>

subscript_expr::subscript_expr(std::unique_ptr<ast_expr> operand, std::unique_ptr<ast_expr> index)
    :operand(std::move(operand)), index(std::move(index))
{
}

void subscript_expr::print(std::ostream& stream) const
{
    stream << "<subscript-expr>:\noperand: ";
    operand->print(stream);
    stream << "\nindex: ";
    index->print(stream);
}

expr_type subscript_expr::get_type() const
{
    return expr_type::SUBSCRIPT;
}

llvm::Value* subscript_expr::codegen(std::shared_ptr<scope> env) const
{
    llvm::Value* gep = no_load_codegen(env);
    return builder.CreateLoad(gep->getType()->getPointerElementType(), gep, "subscriptloadtmp");
}

llvm::Value* subscript_expr::no_load_codegen(std::shared_ptr<scope> env) const
{
        if(operand->get_type() == expr_type::VARIABLE)
    {
        var_expr* left = static_cast<var_expr*>(operand.get());
        llvm::AllocaInst* alloca = find_named_value(left->get_name(), env);
        return builder.CreateInBoundsGEP(alloca->getAllocatedType(), alloca, { llvm::ConstantInt::get(quark_type::i8_t->get_type(), 0), index->codegen(env) },"subscripttmp");
    }
    return nullptr; // TODO: Implement multi-level arrays
}