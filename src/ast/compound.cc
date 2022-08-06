#include <ast/compound.hh>

compound_expr::compound_expr(std::vector<std::unique_ptr<ast_expr>> expressions, std::shared_ptr<scope> env)
    :expressions(std::move(expressions)), env(env)
{
}

void compound_expr::print(std::ostream& stream) const
{
    stream << "<compound-expression>:";
    for(const std::unique_ptr<ast_expr>& expr : expressions)
    {
        stream << "\nexpr: ";
        expr->print(stream);
    }
}

expr_type compound_expr::get_type() const
{
    return expr_type::COMPOUND;
}

llvm::Value* compound_expr::codegen(std::shared_ptr<scope>) const
{
    for(const std::unique_ptr<ast_expr>& expr : expressions)
    {
        expr->codegen(env);
    }
    return nullptr;
}