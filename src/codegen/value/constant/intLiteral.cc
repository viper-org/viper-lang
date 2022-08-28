#include <codegen/value/constant/intLiteral.hh>

namespace Codegen
{
    IntegerLiteral::IntegerLiteral(const long long value, const Module& module)
        :Value(module), _value(value)
    {
    }

    const std::pair<std::string, Register*> IntegerLiteral::Generate(Register* reg)
    {
        if(!reg)
            reg = Register::GetRegister();
        std::string result = "\n\tmovq $" + std::to_string(_value) + ", " + reg->GetID();
        return std::make_pair(result, reg);
    }
}