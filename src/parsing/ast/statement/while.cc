#include <llvm/IR/BasicBlock.h>
#include <parsing/ast/statement/while.hh>

namespace Parsing
{
    WhileStatement::WhileStatement(std::unique_ptr<ASTNode> cond, std::unique_ptr<ASTNode> body)
        :ASTNode(ASTNodeType::IfStatement), _cond(std::move(cond)), _body(std::move(body))
    {
    }

    void WhileStatement::Print(std::ostream& stream, int indent) const
    {
        stream << std::string(indent, ' ') << "<While-Statement>\n";
        stream << "\n" <<std::string(indent, ' ') << "Condition:\n";
        _cond->Print(stream, indent + 2);

        stream << "\n" <<std::string(indent, ' ') << "Body:\n";
        _body->Print(stream, indent + 2);
    }

    llvm::Value* WhileStatement::Emit(llvm::LLVMContext& ctx, llvm::Module& mod, llvm::IRBuilder<>& builder, std::shared_ptr<Environment> scope)
    {
        llvm::Function* func = builder.GetInsertBlock()->getParent();

        llvm::BasicBlock* condBB  = llvm::BasicBlock::Create(ctx, "", func);
        llvm::BasicBlock* bodyBB  = llvm::BasicBlock::Create(ctx);
        llvm::BasicBlock* mergeBB = llvm::BasicBlock::Create(ctx);

        builder.CreateBr(condBB);
        builder.SetInsertPoint(condBB);
        llvm::Value* condValue = _cond->Emit(ctx, mod, builder, scope);
        builder.CreateCondBr(condValue, bodyBB, mergeBB);

        func->getBasicBlockList().push_back(bodyBB);
        builder.SetInsertPoint(bodyBB);
        _body->Emit(ctx, mod, builder, scope);
        builder.CreateBr(condBB);

        func->getBasicBlockList().push_back(mergeBB);
        builder.SetInsertPoint(mergeBB);
        return nullptr;
    }
}