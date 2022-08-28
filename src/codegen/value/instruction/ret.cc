#include <codegen/value/instruction/ret.hh>
#include <iostream>

namespace Codegen
{
    RetInst::RetInst(Value* value, const Module& module)
        :Instruction(module), _value(value)
    {
    }

    const std::pair<std::string, Register*> RetInst::Generate(Register*)
    {
        std::pair<std::string, Register*> valueCodegen = _value->Generate();
        std::string result = valueCodegen.first;
        delete _value;

        if(valueCodegen.second->GetID() == "%rax")
        {
            Register::FreeRegister(valueCodegen.second);
            return std::make_pair(result, nullptr);
        }

        result += "\n\tmovq " + valueCodegen.second->GetID() + ", %rax";
        Register::FreeRegister(valueCodegen.second);

        return std::make_pair(result, nullptr);
    }
}