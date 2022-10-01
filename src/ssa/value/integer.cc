#include <ssa/value/integer.hh>

namespace SSA
{
    IntegerLiteral::IntegerLiteral(long long value)
        :_value(value)
    {
    }

    void IntegerLiteral::Print(std::ostream& stream, int) const
    {
        stream << "int32 " << _value;
    }

    std::unique_ptr<Codegen::Value> IntegerLiteral::Emit(Codegen::Assembly&)
    {
        return std::make_unique<Codegen::ImmediateValue>(_value);
    }
}