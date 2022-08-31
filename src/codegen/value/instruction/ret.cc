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
        if(_value->GetType() != _type)
            _value->SetType(_type);
        
        std::pair<std::string, Register*> valueCodegen = _value->Generate();
        std::string result = valueCodegen.first;

        if(valueCodegen.second->GetID(64) == "%rax")
        {
            delete _value;
            Register::FreeRegister(valueCodegen.second);
            return std::make_pair(result, nullptr);
        }
        Register* reg = Register::FindRegister("%rax");
        result += "\n\tmov" + _value->GetType()->GetSuffix() + " " + valueCodegen.second->GetID(_value->GetType()->GetSize()) + ", " + reg->GetID(_value->GetType()->GetSize());

        delete _value;
        Register::FreeRegister(valueCodegen.second);

        return std::make_pair(result, nullptr);
    }
}