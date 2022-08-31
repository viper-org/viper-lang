#include <codegen/value/instruction/alloca.hh>

namespace Codegen
{
    AllocaInst::AllocaInst(Type* type, int offset, const Module& module)
        :Instruction(module), _offset(offset)
    {
        SetType(type);
    }

    const std::pair<std::string, Register*> AllocaInst::Generate(Register*)
    {
        return std::make_pair(std::to_string(_offset) + "(%rbp)", nullptr);
    }
}