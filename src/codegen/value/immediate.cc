#include <codegen/value/immediate.hh>

namespace Codegen
{
    ImmediateValue::ImmediateValue(long long value)
        :_value(value)
    {
    }

    std::string ImmediateValue::Emit()
    {
        return std::to_string(_value);
    }

    bool ImmediateValue::IsImmediate()
    {
        return true;
    }

    bool ImmediateValue::RequiresSize()
    {
        return true;
    }
}