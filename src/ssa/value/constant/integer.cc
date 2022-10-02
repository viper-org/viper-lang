#include <ssa/value/constant/integer.hh>

namespace SSA
{
    IntegerLiteral::IntegerLiteral(Module& module, long long value)
        :Value(module), _value(value)
    {
    }

    void IntegerLiteral::Print(std::ostream& stream, int) const
    {
        stream << "int32 " << _value;
    }

    std::string IntegerLiteral::GetID() const
    {
        return "int32 " + std::to_string(_value);
    }

    long long IntegerLiteral::GetValue() const
    {
        return _value;
    }

    Codegen::Value* IntegerLiteral::Emit(Codegen::Assembly&)
    {
        return new Codegen::ImmediateValue(_value);
    }
}