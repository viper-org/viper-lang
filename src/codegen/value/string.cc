#include <codegen/value/string.hh>

namespace Codegen
{
    StringLiteral::StringLiteral(const std::string& value, const std::string& name)
        :_value(value), _name(name)
    {
    }

    std::string StringLiteral::Emit(int)
    {
        return "[rel " + _name + "]";
    }

    bool StringLiteral::IsMemory()
    {
        return true;
    }

    void StringLiteral::Dispose()
    {
    }

    std::string_view StringLiteral::GetValue() const
    {
        return _value;
    }

    std::string_view StringLiteral::GetName() const
    {
        return _name;
    }

    int StringLiteral::GetSize() const
    {
        return 64;
    }
}