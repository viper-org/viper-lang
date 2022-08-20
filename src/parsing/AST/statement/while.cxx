#include <llvm/IR/BasicBlock.h>
#include <parsing/AST/statement/while.hxx>

namespace Viper
{
    namespace Parsing
    {
        WhileStatement::WhileStatement(std::unique_ptr<ASTNode> cond, std::unique_ptr<ASTNode> body, std::shared_ptr<Environment> scope)
            :_cond(std::move(cond)), _body(std::move(body)), _scope(scope)
        {
        }

        void WhileStatement::Print(std::ostream& stream) const
        {
            stream << "<While-Statement>:\nCondition: ";
            _cond->Print(stream);
            stream << "\nBody: ";
            _body->Print(stream);
        }

        llvm::Value* WhileStatement::Generate(llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& module, std::shared_ptr<Environment>)
        {
            llvm::Function* function = builder.GetInsertBlock()->getParent();

            llvm::BasicBlock* condBasicBlock  = llvm::BasicBlock::Create(context, "whilecond", function);
            llvm::BasicBlock* bodyBasicBlock  = llvm::BasicBlock::Create(context, "whilebody");
            llvm::BasicBlock* mergeBasicBlock = llvm::BasicBlock::Create(context, "whilemerge");

            builder.CreateBr(condBasicBlock);
            
            builder.SetInsertPoint(condBasicBlock);

            llvm::Value* condValue = _cond->Generate(context, builder, module, _scope);
            condValue = Type::Convert(condValue, llvm::Type::getInt1Ty(context), builder);
            builder.CreateCondBr(condValue, bodyBasicBlock, mergeBasicBlock);

            condBasicBlock = builder.GetInsertBlock();

            function->getBasicBlockList().push_back(bodyBasicBlock);
            builder.SetInsertPoint(bodyBasicBlock);
            _body->Generate(context, builder, module, _scope);
            builder.CreateBr(condBasicBlock);

            bodyBasicBlock = builder.GetInsertBlock();

            function->getBasicBlockList().push_back(mergeBasicBlock);
            builder.SetInsertPoint(mergeBasicBlock);

            return nullptr;
        }
    }
}