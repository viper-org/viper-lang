#include <ast/return.hh>
#include <globals.hh>
#include <iostream>

return_stmt::return_stmt(std::unique_ptr<ast_expr> value)
    :value(std::move(value))
{
    type = this->value->type;
}

void return_stmt::print(std::ostream& stream) const
{
    stream << "<return-statement>: ";
    value->print(stream);
}

expr_type return_stmt::get_type() const
{
    return expr_type::RETURN;
}

llvm::Value* return_stmt::codegen(std::shared_ptr<scope> env) const
{
    llvm::Value* ret_val = value->codegen(env);
    if(ret_val->getType() != type.llvm_getter(ctx))
        ret_val = builder.CreateSExtOrTrunc(ret_val, type.llvm_getter(ctx));
    return builder.CreateRet(ret_val);
}