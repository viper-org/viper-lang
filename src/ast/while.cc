#include "globals.hh"
#include <ast/while.hh>
#include <llvm/IR/Value.h>

while_expr::while_expr(std::unique_ptr<ast_expr> cond, std::unique_ptr<ast_expr> body, std::shared_ptr<scope> env, bool is_do_while)
    :cond(std::move(cond)), body(std::move(body)), env(env), is_do_while(is_do_while)
{
}

void while_expr::print(std::ostream& stream) const
{
    stream << "<while-loop>:\ncond-expr: ";
    cond->print(stream);
    stream << "\nbody: ";
    body->print(stream);
}

expr_type while_expr::get_type() const
{
    return expr_type::WHILE;
}

llvm::Value* while_expr::codegen(std::shared_ptr<scope>) const
{
    llvm::Function* func = builder.GetInsertBlock()->getParent();

    llvm::BasicBlock* cond_bb = llvm::BasicBlock::Create(ctx, "whilecondtmp", func);
    llvm::BasicBlock* body_bb = llvm::BasicBlock::Create(ctx, "whilebodytmp");
    llvm::BasicBlock* end_bb  = llvm::BasicBlock::Create(ctx, "breakmerge");

    env->labels.push_back(end_bb);

    if(is_do_while)
        builder.CreateBr(body_bb);
    else
        builder.CreateBr(cond_bb);

    builder.SetInsertPoint(cond_bb);

    llvm::Value* cond_value = cond->codegen(env);
    cond_value = builder.CreateTrunc(cond_value, llvm::Type::getInt1Ty(ctx));
    builder.CreateCondBr(cond_value, body_bb, end_bb);

    cond_bb = builder.GetInsertBlock();

    func->getBasicBlockList().push_back(body_bb);
    builder.SetInsertPoint(body_bb);
    body->codegen(env);
    builder.CreateBr(cond_bb);

    body_bb = builder.GetInsertBlock();

    func->getBasicBlockList().push_back(end_bb);
    builder.SetInsertPoint(end_bb);

    return nullptr;
}