#include "ast/label.hh"
#include <ast/label.hh>

label_expr::label_expr(std::string label)
    :label(label)
{
}

void label_expr::print(std::ostream& stream) const
{
    stream << "<label-expr>:\nlabel: " << label;
}

expr_type label_expr::get_type() const
{
    return expr_type::LABEL;
}

llvm::Value* label_expr::codegen(std::shared_ptr<scope> env) const
{
    llvm::Function* func = builder.GetInsertBlock()->getParent();

    llvm::BasicBlock* bb = llvm::BasicBlock::Create(ctx, label, func);

    builder.CreateBr(bb);

    builder.SetInsertPoint(bb);

    env->labels.push_back(bb);

    return nullptr;
}