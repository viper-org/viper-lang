#include <parsing/ast/statement/compound.hh>

namespace Parsing
{
    CompoundStatement::CompoundStatement(std::vector<std::unique_ptr<ASTNode>>& statements)
        :ASTNode(ASTNodeType::CompoundStatement), _statements(std::move(statements))
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

    SSA::Value* CompoundStatement::Emit(SSA::Builder& builder)
    {
        for(std::unique_ptr<ASTNode>& statement : _statements)
            statement->Emit(builder);

        return nullptr;
    }
}