#include <codegen/value/instruction/store.hh>

namespace Codegen
{
    StoreInst::StoreInst(Value* value, Value* ptr, const Module& module)
        :Instruction(module), _value(value), _ptr(ptr)
    {
    }

    const std::pair<std::string, Register*> StoreInst::Generate(Register*)
    {
        _value->SetType(_ptr->GetType());
        std::pair<std::string, Register*> valueCodegen = _value->Generate();
        std::string result = valueCodegen.first;

        result += "\n\tmov" + _value->GetType()->GetSuffix() + " " + valueCodegen.second->GetID(_value->GetType()->GetSize()) + ", " + _ptr->Generate().first;

        delete _value;
        Register::FreeRegister(valueCodegen.second);

        return std::make_pair(result, nullptr);
    }
}