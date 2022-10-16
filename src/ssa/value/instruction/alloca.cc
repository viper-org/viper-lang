#include <ssa/value/instruction/alloca.hh>

namespace SSA
{
    AllocaInst::AllocaInst(Module& module, const std::string& name)
        :Instruction(module), _name(new TempValue(module, name))
    {
        _instType = Instruction::Alloca;
    }

    void AllocaInst::Print(std::ostream& stream, int indent) const
    {
        _name->Print(stream, indent);
        stream << "alloca int32\n";
    }

    std::string AllocaInst::GetID() const
    {
        return "int32* " + _name->GetID();
    }

    Codegen::Value* AllocaInst::Emit(Codegen::Assembly&)
    {
        _memory = new Codegen::MemoryValue(_offset, false);
        return _memory;
    }

    void AllocaInst::Dispose()
    {
        _name->Dispose();
        delete _memory;
        delete this;
    }
}