#include <codegen/value/instruction/alloca.hh>

namespace Codegen
{
    AllocaInst::AllocaInst(int offset, const Module& module)
        :Instruction(module), _offset(offset)
    {
    }

    const std::pair<std::string, Register*> AllocaInst::Generate(Register*)
    {
        return std::make_pair(std::to_string(_offset) + "(%rbp)", nullptr);
    }
}