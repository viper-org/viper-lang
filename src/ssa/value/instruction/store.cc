#include <ssa/value/instruction/store.hh>
#include <environment.hh>

namespace SSA
{
    StoreInst::StoreInst(Module& module, Value* ptr, Value* value)
        :Instruction(module), _ptr(ptr), _value(value)
    {
    }

    void StoreInst::Print(std::ostream& stream, int indent) const
    {
        stream << std::string(indent, ' ') << "store " << _value->GetID() << ", " << _ptr->GetID() << '\n';
    }

    Codegen::Value* StoreInst::Emit(Codegen::Assembly& assembly)
    {
        Codegen::Value* ptr = _ptr->Emit(assembly);
        Codegen::Value* value = _value->Emit(assembly);
        assembly.CreateMov(ptr, value);

        delete ptr;
        delete value;

        return value;
    }

    void StoreInst::Dispose()
    {
        _value->Dispose();
        delete this;
    }
}