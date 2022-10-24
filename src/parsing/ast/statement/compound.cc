#include <parsing/ast/statement/compound.hh>

namespace Parsing
{
    CompoundStatement::CompoundStatement(std::vector<std::unique_ptr<ASTNode>>& statements, Environment* scope)
        :ASTNode(ASTNodeType::CompoundStatement), _statements(std::move(statements)), _scope(scope)
    {
    }

    void CompoundStatement::Print(std::ostream& stream, int indent) const
    {
        stream << std::string(indent, ' ') << "<Compound-Statement>:";
        for(const std::unique_ptr<ASTNode>& statement : _statements)
        {
            stream << "\n";
            statement->Print(stream, indent + 2);
        }
    }

    SSA::Value* CompoundStatement::Emit(SSA::Builder& builder, Environment*, bool isStatement)
    {
        for(std::unique_ptr<ASTNode>& statement : _statements)
            statement->Emit(builder, _scope, isStatement);
        
        delete _scope;

        return nullptr;
    }
}