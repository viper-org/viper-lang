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

    SSA::Value* IfStatement::Emit(SSA::Builder& builder, Environment* scope, bool)
    {
        bool hasElse = (_elseBody != nullptr);

        SSA::Value* condValue = _cond->Emit(builder, scope);
        
        SSA::Function* func = builder.GetInsertPoint()->GetParent();

        SSA::BasicBlock* thenBB = SSA::BasicBlock::Create(builder.GetModule(), func);
        SSA::BasicBlock* elseBB;
        if(hasElse)
            elseBB = SSA::BasicBlock::Create(builder.GetModule(), func);
        
        SSA::BasicBlock* mergeBB = SSA::BasicBlock::Create(builder.GetModule(), func);

        if(hasElse)
            builder.CreateCondJmp(condValue, elseBB, thenBB);
        else
            builder.CreateCondJmp(condValue, mergeBB, thenBB);
        //if(hasElse)
        //    builder.CreateJmp(elseBB);
        //else
        //    builder.CreateJmp(mergeBB);

        builder.SetInsertPoint(thenBB);
        _body->Emit(builder, scope, true);
        if(hasElse)
        {
            builder.CreateJmp(mergeBB);

            builder.SetInsertPoint(elseBB);

            _elseBody->Emit(builder, scope, true);
        }

        builder.SetInsertPoint(mergeBB);
        
        return nullptr;
    }
}