#include <ssa/value/tmpValue.hh>

namespace SSA
{
    TempValue::TempValue(Module& module, const std::string& name)
        :Value(module), _name(name)
    {
        if(_name == "")
            _name = std::to_string(module.GetNextInstName());
    }

    std::string TempValue::GetID() const
    {
        return "%" + _name;
    }

    void TempValue::Print(std::ostream& stream, int indent) const
    {
        stream << std::string(indent, ' ') << "%" << _name << " = ";
    }

    Codegen::Value* TempValue::Emit(Codegen::Assembly&)
    {
        return nullptr;
    }

    void TempValue::Dispose()
    {
        delete this;
    }
}