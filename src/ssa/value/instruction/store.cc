#include <ssa/value/instruction/store.hh>

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

    Codegen::Value* StoreInst::Emit(Codegen::Assembly&)
    {
        return nullptr;
    }

    void StoreInst::Dispose()
    {
        _value->Dispose();
        delete this;
    }
}