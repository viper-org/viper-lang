#include <ast/integer.hh>
#include <globals.hh>
#include <typing/types.hh>
#include <llvm/ADT/APInt.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/Type.h>

integer_expr::integer_expr(int value)
    :value(value)
{
    type = types.at("i32");
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
    return llvm::ConstantInt::get(type.get_llvm_type(), value);
}