#include <ssa/function.hh>

namespace SSA
{
    Function::Function(const std::string& name, std::unique_ptr<Value>& body)
        :_name(name), _body(std::move(body))
    {
    }

    void Function::Print(std::ostream& stream, int indent) const
    {
        stream << std::string(indent, ' ') << "define int32 " << _name << "() {\n";
        _body->Print(stream, indent + 2);
        stream << std::string(indent, ' ') << "}";
    }

    std::unique_ptr<Codegen::Value> Function::Emit(Codegen::Assembly& assembly)
    {
        assembly.CreateGlobal(_name);
        assembly.CreateLabel(_name);

        return _body->Emit(assembly);
    }
}