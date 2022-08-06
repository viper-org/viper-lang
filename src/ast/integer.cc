#include <ast/integer.hh>
#include <globals.hh>
#include <llvm/ADT/APInt.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/Type.h>

integer_expr::integer_expr(int value)
    :value(value)
{
    type = quark_type::i32_t;
}

void integer_expr::print(std::ostream& stream) const
{
    stream << "<integer-literal>: " << std::to_string(value);
}

expr_type integer_expr::get_type() const
{
    return expr_type::INT_LITERAL;
}

llvm::Value* integer_expr::codegen(std::shared_ptr<scope>) const
{
    return llvm::ConstantInt::get(type->get_type(), value);
}