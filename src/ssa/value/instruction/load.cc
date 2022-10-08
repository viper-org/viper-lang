#include <ssa/value/instruction/load.hh>
#include <environment.hh>

namespace SSA
{
    LoadInst::LoadInst(Module& module, Value* ptr, const std::string& name)
        :Instruction(module), _ptr(ptr), _name(name)
    {
        if(_name == "")
            _name = std::to_string(module.GetNextInstName());
    }

    std::string LoadInst::GetID() const
    {
        return "%" + _name;
    }

    void LoadInst::Print(std::ostream& stream, int indent) const
    {
        stream << std::string(indent, ' ') << "%" << _name << " = load int32, " << _ptr->GetID() << '\n';
    }

    Codegen::Value* LoadInst::Emit(Codegen::Assembly& assembly)
    {
        Codegen::Value* ptr = _ptr->Emit(assembly);
        Codegen::MemoryValue* mem = static_cast<Codegen::MemoryValue*>(ptr);
        Codegen::MemoryValue* ref = new Codegen::MemoryValue(mem, true);

        delete ptr;

        return ref;
    }

    void LoadInst::Dispose()
    {
        delete this;
    }
}