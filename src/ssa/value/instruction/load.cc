#include <ssa/value/instruction/load.hh>
#include <environment.hh>

namespace SSA
{
    LoadInst::LoadInst(Module& module, Value* ptr, const std::string& name)
        :Instruction(module), _name(new TempValue(module, name)), _ptr(ptr)
    {
        _instType = Instruction::Load;
    }

    std::string LoadInst::GetID() const
    {
        return _name->GetID();
    }

    void LoadInst::Print(std::ostream& stream, int indent) const
    {
        _name->Print(stream, indent);
        stream << "load int32, " << _ptr->GetID() << '\n';
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
        _name->Dispose();
        delete this;
    }
}