#include <ast/if.hh>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Value.h>
#include <typing/types.hh>

if_expr::if_expr(std::unique_ptr<ast_expr> condition, std::unique_ptr<ast_expr> body, std::unique_ptr<ast_expr> else_body, std::shared_ptr<scope> env, std::shared_ptr<scope> else_env)
    :condition(std::move(condition)), body(std::move(body)), else_body(std::move(else_body)), env(env), else_env(else_env)
{
}

void if_expr::print(std::ostream& stream) const
{
    stream << "<if-expression>:\ncondition: ";
    condition->print(stream);
    stream << "\nbody: ";
    body->print(stream);
}

expr_type if_expr::get_type() const
{
    return expr_type::IF;
}

llvm::Value* if_expr::codegen(std::shared_ptr<scope>) const
{
    bool has_else = (else_body != nullptr);
    llvm::Value* condition_value = condition->codegen(env);

    condition_value = type_info::convert(condition_value, llvm::Type::getInt1Ty(ctx));

    llvm::Function* func = builder.GetInsertBlock()->getParent();

    llvm::BasicBlock* then_bb = llvm::BasicBlock::Create(ctx, "ifthentmp", func);
    llvm::BasicBlock* else_bb;
    if(has_else)
        else_bb = llvm::BasicBlock::Create(ctx, "ifelsetmp");
    llvm::BasicBlock* merge_bb = llvm::BasicBlock::Create(ctx, "ifmergetmp");

    if(has_else)
        builder.CreateCondBr(condition_value, then_bb, else_bb);
    else
        builder.CreateCondBr(condition_value, then_bb, merge_bb);

    builder.SetInsertPoint(then_bb);

    llvm::Value* body_value = body->codegen(env);

    builder.CreateBr(merge_bb);

    then_bb = builder.GetInsertBlock();

    if(has_else)
    {
        func->getBasicBlockList().push_back(else_bb);
        builder.SetInsertPoint(else_bb);
    }

    llvm::Value* else_body_value = nullptr;
    if(has_else)
        else_body->codegen(else_env);

    if(has_else)
    {
        builder.CreateBr(merge_bb);
        else_bb = builder.GetInsertBlock();
    }

    func->getBasicBlockList().push_back(merge_bb);
    builder.SetInsertPoint(merge_bb);
    return nullptr;
}