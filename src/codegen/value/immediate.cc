#include <codegen/value/immediate.hh>

namespace Codegen
{
    ImmediateValue::ImmediateValue(long long value, std::shared_ptr<Type> type)
        :_value(value), _type(type)
    {
    }

    std::string ImmediateValue::Emit(int)
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

    int ImmediateValue::GetSize() const
    {
        return _type->GetPrimitiveSize();
    }
}