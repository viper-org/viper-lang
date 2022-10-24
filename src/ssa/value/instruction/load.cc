#include <ssa/value/instruction/load.hh>
#include <environment.hh>

namespace SSA
{
    LoadInst::LoadInst(Module& module, Value* ptr, const std::string& name)
        :Instruction(module), _name(new TempValue(module, name)), _memory(nullptr), _ptr(ptr)
    {
        _instType = Instruction::Load;
    }

    std::string LoadInst::GetID() const
    {
        return "int32 " + _name->GetID();
    }

    void LoadInst::Print(std::ostream& stream, int indent) const
    {
        _name->Print(stream, indent);
        stream << "load int32, " << _ptr->GetID() << '\n';
    }

    Codegen::Value* LoadInst::Emit(Codegen::Assembly& assembly)
    {
        Codegen::Value* ptr = _ptr->Emit(assembly);
        if(dynamic_cast<LoadInst*>(_ptr))
        {
            Codegen::Register* reg = Codegen::Register::AllocRegister(Codegen::RegisterType::Integral);
            assembly.CreateMov(reg, ptr);
            assembly.CreateIndirect(reg, reg);
            ptr->Dispose();
            _ptr->Dispose();
            return reg;
        }
        _memory = new Codegen::MemoryValue(static_cast<Codegen::MemoryValue*>(ptr), true);

        ptr->Dispose();

        return _memory;
    }

    void LoadInst::Dispose()
    {
        _name->Dispose();
        if(_memory)
            delete _memory;
        delete this;
    }
}