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

    void Assembly::CreateMov(std::unique_ptr<Value> left, std::unique_ptr<Value> right)
    {
        VerifyArgs(left, right);

        _output << "\n\tmov " << left->Emit() << ", " << right->Emit();
    }

    void Assembly::CreateRet()
    {
        _output << "\n\tret";
    }

    void Assembly::VerifyArgs(std::unique_ptr<Value>&, std::unique_ptr<Value>&)
    {
        // TODO: Verify args
    }
}