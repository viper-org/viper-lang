#include "parsing/ast/astNode.hh"
#include <parsing/ast/statement/import.hh>

namespace Parsing
{
    ImportStatement::ImportStatement(const std::string& name, std::shared_ptr<Type> type, std::vector<std::pair<std::shared_ptr<Type>, std::string>> args)
        :ASTNode(ASTNodeType::ImportStatement), _name(name), _args(args)
    {
        _type = type;
    }

    void ImportStatement::Print(std::ostream& stream, int indent) const
    {
        stream << std::string(indent, ' ') << "<Import>:\n";
        stream << std::string(indent, ' ') << "Name: " << _name;
    }

    SSA::Value* ImportStatement::Emit(SSA::Builder& builder, Environment*, bool)
    {
        SSA::Function* func = SSA::Function::Create(builder.GetModule(), _name, std::vector<SSA::AllocaInst*>(0));
        return func;
    }
}