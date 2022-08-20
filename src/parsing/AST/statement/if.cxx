#include <llvm/IR/BasicBlock.h>
#include <parsing/AST/statement/if.hxx>

namespace Viper
{
    namespace Parsing
    {
        IfStatement::IfStatement(std::unique_ptr<ASTNode> cond,
            std::shared_ptr<Environment> scope, std::unique_ptr<ASTNode> body,
            std::shared_ptr<Environment> elseScope, std::unique_ptr<ASTNode> elseBody)
            :_cond(std::move(cond)), _scope(scope), _body(std::move(body)), _elseScope(elseScope), _elseBody(std::move(elseBody))
        {
            _nodeType = ASTNodeType::IfStatement;
        }

        void IfStatement::Print(std::ostream& stream) const
        {
            stream << "<If-Statement>:\nCondition: ";
            _cond->Print(stream);
            stream << "\nBody: ";
            _body->Print(stream);
        }

        llvm::Value* IfStatement::Generate(llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& module, std::shared_ptr<Environment> scope)
        {
            bool hasElse = (_elseBody != nullptr);

            llvm::Value* condValue = _cond->Generate(context, builder, module, scope);
            condValue = Type::Convert(condValue, llvm::Type::getInt1Ty(context), builder);

            llvm::Function* function = builder.GetInsertBlock()->getParent();

            llvm::BasicBlock* thenBasicBlock = llvm::BasicBlock::Create(context, "ifthen", function);
            llvm::BasicBlock* elseBasicBlock;
            if(hasElse)
                elseBasicBlock = llvm::BasicBlock::Create(context, "ifelse");
            llvm::BasicBlock* mergeBasicBlock = llvm::BasicBlock::Create(context, "ifmerge");

            if(hasElse)
                builder.CreateCondBr(condValue, thenBasicBlock, elseBasicBlock);
            else
                builder.CreateCondBr(condValue, thenBasicBlock, mergeBasicBlock);

            builder.SetInsertPoint(thenBasicBlock);
            _body->Generate(context, builder, module, _scope);
            builder.CreateBr(mergeBasicBlock);

            thenBasicBlock = builder.GetInsertBlock();

            if(hasElse)
            {
                function->getBasicBlockList().push_back(elseBasicBlock);
                builder.SetInsertPoint(elseBasicBlock);
            }

            if(hasElse)
            {
                _elseBody->Generate(context, builder, module, scope);
                builder.CreateBr(mergeBasicBlock);
                elseBasicBlock = builder.GetInsertBlock();
            }

            function->getBasicBlockList().push_back(mergeBasicBlock);
            builder.SetInsertPoint(mergeBasicBlock);
            return nullptr;
        }
    }
}