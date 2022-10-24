#include <memory>
#include <parsing/ast/expression/variable.hh>
#include <environment.hh>

namespace Parsing
{
    Variable::Variable(std::string name, std::shared_ptr<Type> type)
        :ASTNode(ASTNodeType::Variable), _name(name)
    {
        _type = type;
    }

    void Variable::Print(std::ostream& stream, int indent) const
    {
        stream << std::string(indent, ' ') << "<Variable>: " << _name;
    }

    SSA::Value* Variable::Emit(SSA::Builder& builder, bool)
    {
        SSA::AllocaInst* ptr = namedValues[_name];
        return builder.CreateLoad(ptr, "");
    }

    std::string Variable::GetName() const
    {
        return _name;
    }
}