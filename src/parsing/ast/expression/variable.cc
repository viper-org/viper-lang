#include <parsing/ast/expression/variable.hh>
#include <environment.hh>

namespace Parsing
{
    Variable::Variable(std::string name)
        :_name(name)
    {
        _nodeType = ASTNodeType::Variable;
    }

    void Variable::Print(std::ostream& stream, int indent) const
    {
        stream << std::string(indent, ' ') << "<Variable: " << _name;
    }

    SSA::Value* Variable::Emit(SSA::Builder& builder)
    {
        SSA::AllocaInst* ptr = namedValues[_name];
        return builder.CreateLoad(ptr, "");
    }
}