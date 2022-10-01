#include <parsing/ast/statement/variableDeclaration.hh>

namespace Parsing
{
    VariableDeclaration::VariableDeclaration(const std::string& name, std::unique_ptr<ASTNode> initVal, bool isFunction)
        :_name(name), _initVal(std::move(initVal)), _isFunction(isFunction)
    {
        _nodeType = (_isFunction ? ASTNodeType::Function : ASTNodeType::VariableDeclaration);
    }

    void VariableDeclaration::Print(std::ostream& stream, int indent) const
    {
        stream << std::string(indent, ' ') << (_isFunction ? "<Function>:\n" : "<Variable-Declaration>:\n");
        stream << std::string(indent, ' ') << "Name: " << _name;
        if(_initVal)
        {
            stream << "\n" << std::string(indent, ' ') << "Value: \n";
            _initVal->Print(stream, indent + 2);
        }
    }

    bool VariableDeclaration::IsFunction() const
    {
        return _isFunction;
    }

    std::unique_ptr<SSA::Value> VariableDeclaration::Emit()
    {
        std::unique_ptr<SSA::Value> initVal = _initVal->Emit();
        if(_isFunction)
            return std::make_unique<SSA::Function>(_name, initVal);
        
        return nullptr; // TODO: Add variables
    }
}