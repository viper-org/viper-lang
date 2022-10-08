#include <ssa/value/instruction/alloca.hh>

namespace SSA
{
    AllocaInst::AllocaInst(Module& module, const std::string& name)
        :Instruction(module), _name(name)
    {
        if(_name == "")
            _name = std::to_string(module.GetNextInstName());
    }

    void AllocaInst::Print(std::ostream& stream, int indent) const
    {
        stream << std::string(indent, ' ') << "%" << _name << " = alloca int32\n";
    }

    std::string AllocaInst::GetID() const
    {
        return "int32* %" + _name;
    }

    Codegen::Value* AllocaInst::Emit(Codegen::Assembly&)
    {
        Codegen::MemoryValue* memory = new Codegen::MemoryValue(_offset, false);
        return memory;
    }

    void AllocaInst::Dispose()
    {
        delete this;
    }
}