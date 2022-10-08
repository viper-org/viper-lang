#include <ssa/value/instruction/return.hh>

namespace SSA
{
    RetInst::RetInst(Module& module, Value* value)
        :Instruction(module), _value(value)
    {
        _instType = InstType::Ret;
    }

    void RetInst::Print(std::ostream& stream, int indent) const
    {
        if(_value)
            _value->Print(stream, indent);
        stream << std::string(indent, ' ') << "ret ";
        if(_value)
            stream << _value->GetID();
        else
            stream << "void";
        
        stream << '\n';
    }

    Codegen::Value* RetInst::Emit(Codegen::Assembly& assembly)
    {
        if(_value)
        {
            Codegen::Value* value = _value->Emit(assembly);
            Codegen::Value* rax = new Codegen::Register(Codegen::Registers::RAX);

            assembly.CreateMov(rax, std::move(value));

            delete rax;
            delete value;
        }
        
        assembly.CreateJmp(".ret");

        return nullptr;
    }

    void RetInst::Dispose()
    {
        if(_value)
            _value->Dispose();
        delete this;
    }
}