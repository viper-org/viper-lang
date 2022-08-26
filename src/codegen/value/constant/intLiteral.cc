#include <codegen/value/constant/intLiteral.hh>

namespace Codegen
{
    IntegerLiteral::IntegerLiteral(const long long value, const Module& module)
        :Value(module), _value(value)
    {
    }

    const std::string IntegerLiteral::Generate()
    {
        return std::to_string(_value);
    }
}