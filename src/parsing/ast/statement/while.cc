#include <parsing/ast/statement/while.hh>

namespace Parsing
{
    WhileStatement::WhileStatement(std::unique_ptr<ASTNode> cond, std::unique_ptr<ASTNode> body)
        :ASTNode(ASTNodeType::WhileStatement), _cond(std::move(cond)), _body(std::move(body))
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

    SSA::Value* WhileStatement::Emit(SSA::Builder& builder, Environment* scope, bool)
    {
        SSA::Function* func = builder.GetInsertPoint()->GetParent();

        SSA::BasicBlock* bodyBB = SSA::BasicBlock::Create(builder.GetModule(), func);
        SSA::BasicBlock* condBB = SSA::BasicBlock::Create(builder.GetModule(), func);

        builder.CreateJmp(condBB);
        builder.SetInsertPoint(bodyBB);
        _body->Emit(builder, scope, true);

        builder.SetInsertPoint(condBB);
        SSA::Value* condValue = _cond->Emit(builder, scope, true);
        builder.CreateCondJmp(condValue, bodyBB, nullptr);

        return nullptr;
    }
}