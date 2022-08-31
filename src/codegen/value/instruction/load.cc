#include <codegen/value/instruction/load.hh>

namespace Codegen
{
    LoadInst::LoadInst(Value* ptr, const Module& module)
        :Instruction(module), _ptr(ptr)
    {
        SetType(_ptr->GetType());
    }

    const std::pair<std::string, Register*> LoadInst::Generate(Register* reg)
    {
        if(!reg)
            reg = Register::GetRegister();
        std::string result = "\n\tmov" + _ptr->GetType()->GetSuffix() + " " + _ptr->Generate().first + ", " + reg->GetID(_ptr->GetType()->GetSize());

        return std::make_pair(result, reg);
    }

    Value* LoadInst::GetPtr() const
    {
        return _ptr;
    }
}