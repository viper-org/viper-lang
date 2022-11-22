#include <parsing/ast/statement/if.hh>

namespace Parsing
{
    IfStatement::IfStatement(std::unique_ptr<ASTNode> cond, std::unique_ptr<ASTNode> body, std::unique_ptr<ASTNode> elseBody)
        :ASTNode(ASTNodeType::IfStatement), _cond(std::move(cond)), _body(std::move(body)), _elseBody(std::move(elseBody))
    {
    }

    void IfStatement::Print(std::ostream& stream, int indent) const
    {
        stream << std::string(indent, ' ') << "<If-Statement>\n";
        stream << "\n" <<std::string(indent, ' ') << "Condition:\n";
        _cond->Print(stream, indent + 2);

        stream << "\n" <<std::string(indent, ' ') << "Body:\n";
        _body->Print(stream, indent + 2);
        
        stream << "\n" <<std::string(indent, ' ') << "Else-Body:\n";
        _elseBody->Print(stream, indent + 2);
    }

    llvm::Value* IfStatement::Emit(llvm::LLVMContext& ctx, llvm::Module& mod, llvm::IRBuilder<>& builder, std::shared_ptr<Environment> scope)
    {
        bool hasElse = (_elseBody != nullptr);

        llvm::Value* condValue = _cond->Emit(ctx, mod, builder, scope);

        condValue = Type::Convert(condValue, types.at("bool")->GetLLVMType(), builder);
        
        llvm::Function* func = builder.GetInsertBlock()->getParent();

        llvm::BasicBlock* thenBB = llvm::BasicBlock::Create(ctx, "", func);
        llvm::BasicBlock* elseBB;
        if(hasElse)
            elseBB = llvm::BasicBlock::Create(ctx);
        
        llvm::BasicBlock* mergeBB = llvm::BasicBlock::Create(ctx);

        if(hasElse)
            builder.CreateCondBr(condValue, thenBB, elseBB);
        else
            builder.CreateCondBr(condValue, thenBB, mergeBB);

        builder.SetInsertPoint(thenBB);
        _body->Emit(ctx, mod, builder, scope);
        if(hasElse)
        {
            if(!builder.GetInsertBlock()->getInstList().back().isTerminator())
                builder.CreateBr(mergeBB);
            
            func->getBasicBlockList().push_back(elseBB);
            builder.SetInsertPoint(elseBB);

            _elseBody->Emit(ctx, mod, builder, scope);
        }

        func->getBasicBlockList().push_back(mergeBB);
        builder.SetInsertPoint(mergeBB);
        
        return nullptr;
    }
}