#include <llvm/IR/BasicBlock.h>
#include <parsing/AST/statement/for.hxx>

namespace Viper
{
    namespace Parsing
    {
        ForStatement::ForStatement(std::unique_ptr<ASTNode> init, std::unique_ptr<ASTNode> cond, std::unique_ptr<ASTNode> iter, std::unique_ptr<ASTNode> body, std::shared_ptr<Environment> scope)
            :_init(std::move(init)), _cond(std::move(cond)), _iter(std::move(iter)), _body(std::move(body)), _scope(scope)
        {
        }

        void ForStatement::Print(std::ostream& stream) const
        {
            stream << "<While-Statement>:\nInit: ";
            _init->Print(stream);

            stream << "\nCondition: ";
            _cond->Print(stream);

            stream << "\nIterate: ";
            _iter->Print(stream);

            stream << "\nBody: ";
            _body->Print(stream);
        }

        llvm::Value* ForStatement::Generate(llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& module, std::shared_ptr<Environment>)
        {
            _init->Generate(context, builder, module, _scope);

            llvm::Function* function = builder.GetInsertBlock()->getParent();

            llvm::BasicBlock* condBasicBlock = llvm::BasicBlock::Create(context, "forcond", function);
            llvm::BasicBlock* bodyBasicBlock = llvm::BasicBlock::Create(context, "forbody");
            llvm::BasicBlock* mergeBasicBlock = llvm::BasicBlock::Create(context, "breakmerge");

            _scope->labels.push_back(mergeBasicBlock);

            builder.CreateBr(condBasicBlock);
            builder.SetInsertPoint(condBasicBlock);

            llvm::Value* condValue = _cond->Generate(context, builder, module, _scope);
            condValue = Type::Convert(condValue, llvm::Type::getInt1Ty(context), builder);
            builder.CreateCondBr(condValue, bodyBasicBlock, mergeBasicBlock);

            condBasicBlock = builder.GetInsertBlock();

            function->getBasicBlockList().push_back(bodyBasicBlock);
            builder.SetInsertPoint(bodyBasicBlock);
            _body->Generate(context, builder, module, _scope);
            _iter->Generate(context, builder, module, _scope);
            builder.CreateBr(condBasicBlock);

            bodyBasicBlock = builder.GetInsertBlock();

            function->getBasicBlockList().push_back(mergeBasicBlock);
            builder.SetInsertPoint(mergeBasicBlock);

            return nullptr;
        }
    }
}