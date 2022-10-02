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

    Codegen::Value* AllocaInst::Emit(Codegen::Assembly&)
    {
        return nullptr;
    }

    void AllocaInst::Dispose()
    {
        delete this;
    }
}