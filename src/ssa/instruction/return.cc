#include <ssa/instruction/return.hh>

namespace SSA
{
    Return::Return(std::unique_ptr<Value>& value)
        :_value(std::move(value))
    {
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

    std::unique_ptr<Codegen::Value> Return::Emit(Codegen::Assembly& assembly)
    {
        if(_value)
        {
            std::unique_ptr<Codegen::Value> value = _value->Emit(assembly);
            std::unique_ptr<Codegen::Value> rax = std::make_unique<Codegen::Register>(Codegen::Registers::RAX);

            assembly.CreateMov(std::move(rax), std::move(value));
        }
        
        assembly.CreateRet();

        return nullptr;
    }
}