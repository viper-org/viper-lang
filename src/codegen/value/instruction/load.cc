#include <codegen/value/instruction/load.hh>

namespace Codegen
{
    LoadInst::LoadInst(Value* ptr, const Module& module)
        :Instruction(module), _ptr(ptr)
    {
    }

    const std::pair<std::string, Register*> LoadInst::Generate(Register* reg)
    {
        if(!reg)
            reg = Register::GetRegister();
        std::string result = "\n\tmovq " + _ptr->Generate().first + ", " + reg->GetID();

        return std::make_pair(result, reg);
    }
}