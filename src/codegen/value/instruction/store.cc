#include <codegen/value/instruction/store.hh>

namespace Codegen
{
    StoreInst::StoreInst(Value* value, Value* ptr, const Module& module)
        :Instruction(module), _value(value), _ptr(ptr)
    {
    }

    const std::pair<std::string, Register*> StoreInst::Generate(Register*)
    {
        std::pair<std::string, Register*> valueCodegen = _value->Generate();
        std::string result = valueCodegen.first;
        delete _value;

        result += "\n\tmovq " + valueCodegen.second->GetID() + ", " + _ptr->Generate().first;
        Register::FreeRegister(valueCodegen.second);

        return std::make_pair(result, nullptr);
    }
}