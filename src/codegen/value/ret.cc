#include <codegen/value/ret.hh>
#include <iostream>

namespace Codegen
{
    RetVal::RetVal(Value* value, const Module& module)
        :Value(module), _value(value)
    {
    }

    const std::string RetVal::Generate()
    {
        std::string result = "\n\tmovl " + _value->Generate() + ", %eax";
        delete _value;

        return result;
    }
}