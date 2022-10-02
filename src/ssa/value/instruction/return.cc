#include <ssa/value/instruction/return.hh>

namespace SSA
{
    Return::Return(Module& module, Value* value)
        :Instruction(module), _value(value)
    {
        _instType = InstType::Return;
    }

    void Return::Print(std::ostream& stream, int indent) const
    {
        stream << std::string(indent, ' ') << "ret ";
        if(_value)
            _value->Print(stream, indent);
        else
            stream << "void";
        
        stream << '\n';
    }

    Codegen::Value* Return::Emit(Codegen::Assembly& assembly)
    {
        if(_value)
        {
            Codegen::Value* value = _value->Emit(assembly);
            Codegen::Value* rax = new Codegen::Register(Codegen::Registers::RAX);

            assembly.CreateMov(rax, std::move(value));

            delete rax;
            delete value;
        }
        
        assembly.CreateRet();

        return nullptr;
    }

    void Return::Dispose()
    {
        _value->Dispose();
        delete this;
    }
}