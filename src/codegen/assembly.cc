#include <codegen/assembly.hh>

namespace Codegen
{
    Assembly::Assembly()
    {
    }

    void Assembly::Emit(std::ostream& stream)
    {
        stream << _output.view() << "\n";
    }

    void Assembly::CreateGlobal(std::string_view ident)
    {
        _output << "\n\t[global " << ident << "]";
    }

    void Assembly::CreateLabel(std::string_view ident)
    {
        _output << "\n" << ident << ":";
    }

    void Assembly::CreateMov(Value* left, Value* right)
    {
        VerifyArgs(left, right);

        _output << "\n\tmov " << left->Emit() << ", " << right->Emit();
    }

    void Assembly::CreateRet()
    {
        _output << "\n\tret";
    }

    void Assembly::VerifyArgs(Value*, Value*)
    {
        // TODO: Verify args
    }
}