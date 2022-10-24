#include <ssa/value/instruction/store.hh>
#include <environment.hh>

namespace SSA
{
    StoreInst::StoreInst(Module& module, Value* ptr, Value* value)
        :Instruction(module), _ptr(ptr), _value(value)
    {
        _instType = Instruction::Store;
    }

    void StoreInst::Print(std::ostream& stream, int indent) const
    {
        stream << std::string(indent, ' ') << "store " << _value->GetID() << ", " << _ptr->GetID() << '\n';
    }

    Codegen::Value* StoreInst::Emit(Codegen::Assembly& assembly)
    {
        Codegen::Value* ptr = _ptr->Emit(assembly);
        Codegen::Value* value = _value->Emit(assembly);
        if(value->IsMemory())
        {
            Codegen::Register* reg = Codegen::Register::AllocRegister(Codegen::RegisterType::Integral);
            assembly.CreateMov(reg, value);
            value->Dispose();
            value = reg;
        }
        
        assembly.CreateMov(ptr, value);

        value->Dispose();

        return nullptr;
    }

    void StoreInst::Dispose()
    {
        _value->Dispose();
        delete this;
    }
}