#include "globals.hh"
#include <ast/break.hh>
#include <algorithm>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/SymbolTableListTraits.h>

break_expr::break_expr()
{
}

void break_expr::print(std::ostream& stream) const
{
    stream << "<break-expr>";
}

expr_type break_expr::get_type() const
{
    return expr_type::BREAK;
}

llvm::Value* break_expr::codegen(std::shared_ptr<scope> env) const
{
    return builder.CreateBr(find_label("breakmerge", env));
}