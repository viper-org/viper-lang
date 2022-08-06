#include <ast/goto.hh>

goto_expr::goto_expr(std::string label)
    :label(label)
{
}

void goto_expr::print(std::ostream& stream) const
{
    stream << "<goto-expr>:\nlabel: " << label;
}

expr_type goto_expr::get_type() const
{
    return expr_type::GOTO;
}

llvm::Value* goto_expr::codegen(std::shared_ptr<scope> env) const
{
    return builder.CreateBr(find_label(label, env));
}