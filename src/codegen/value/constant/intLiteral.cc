#include <codegen/value/constant/intLiteral.hh>
#include <iostream>

namespace Codegen
{
    IntegerLiteral::IntegerLiteral(const long long value, Type* type, const Module& module)
        :Value(module), _value(value)
    {
        _type = type;
    }

    const std::pair<std::string, Register*> IntegerLiteral::Generate(Register* reg)
    {
        if(!reg)
            reg = Register::GetRegister();
        std::string result;
        if(_value == 0)
            result =  "\n\txor" + _type->GetSuffix() + " " + reg->GetID(_type->GetSize()) + ", " + reg->GetID(_type->GetSize());
        else
            result = "\n\tmov" + _type->GetSuffix() + " $" + std::to_string(_value) + ", " + reg->GetID(_type->GetSize());
        return std::make_pair(result, reg);
    }
}