#include <codegen/value/instruction/ret.hh>
#include <iostream>

namespace Codegen
{
    RetVal::RetVal(Value* value, const Module& module)
        :Instruction(module), _value(value)
    {
    }

    const std::string RetVal::Generate()
    {
        std::string result = _value->Generate();
        delete _value;

        result += "\n\tpopq %rax";

        return result + "\n\tjmp .return";
    }
}