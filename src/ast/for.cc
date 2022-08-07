#include <ast/for.hh>
#include <llvm/IR/BasicBlock.h>
#include <iostream>

for_expr::for_expr(std::unique_ptr<ast_expr> init, std::unique_ptr<ast_expr> cond, std::unique_ptr<ast_expr> iter, std::shared_ptr<scope> env, std::unique_ptr<ast_expr> body)
    :init(std::move(init)), cond(std::move(cond)), iter(std::move(iter)), env(env), body(std::move(body))
{
}

void for_expr::print(std::ostream& stream) const
{
    stream << "<for-loop>:\ninit-expr: ";
    init->print(stream);
    stream << "\ncond-expr: ";
    cond->print(stream);
    stream << "\niter-expr: ";
    iter->print(stream);
    stream << "\nbody: ";
    body->print(stream);
}

expr_type for_expr::get_type() const
{
    return expr_type::FOR;
}

llvm::Value* for_expr::codegen(std::shared_ptr<scope>) const
{
    init->codegen(env);

    llvm::Function* func = builder.GetInsertBlock()->getParent();

    llvm::BasicBlock* cond_bb = llvm::BasicBlock::Create(ctx, "forcondtmp", func);
    llvm::BasicBlock* body_bb = llvm::BasicBlock::Create(ctx, "forbodytmp");
    llvm::BasicBlock* end_bb  = llvm::BasicBlock::Create(ctx, "breakmerge");

    env->labels.push_back(end_bb);

    builder.CreateBr(cond_bb);
    builder.SetInsertPoint(cond_bb);

    llvm::Value* cond_value = cond->codegen(env);
    cond->print(std::cout);
    cond_value = quark_type::convert(cond_value, llvm::Type::getInt1Ty(ctx));
    builder.CreateCondBr(cond_value, body_bb, end_bb);

    cond_bb = builder.GetInsertBlock();

    func->getBasicBlockList().push_back(body_bb);
    builder.SetInsertPoint(body_bb);
    body->codegen(env);
    iter->codegen(env);
    builder.CreateBr(cond_bb);

    body_bb = builder.GetInsertBlock();

    func->getBasicBlockList().push_back(end_bb);
    builder.SetInsertPoint(end_bb);

    return nullptr;
}