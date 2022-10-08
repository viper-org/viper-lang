#include <codegen/assembly.hh>
#include <diagnostics.hh>
#include <array>

namespace Codegen
{
    using namespace std::literals;

    constexpr std::array op_sizes {
        "BYTE"sv,
        "WORD"sv,
        "DWORD"sv,
        "QWORD"sv,
    };

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


    void Assembly::CreateJmp(std::string_view label)
    {
        _output << "\n\tjmp " << label;
    }


    void Assembly::CreatePush(Value* operand)
    {
        if(operand->RequiresSize())
            _output << "\n\tpush " << op_sizes[2] << ' ' << operand->Emit();
        else
            _output << "\n\tpush " << operand->Emit();
    }


    void Assembly::CreateBinOp(Value* left, Value* right, std::string_view op)
    {
        VerifyArgs(left, right);

        if(left->RequiresSize() || right->RequiresSize())
            _output << "\n\t" << op << ' ' << op_sizes[2] << ' ' << left->Emit() << ", " << right->Emit();

        _output << "\n\t" << op << ' ' << left->Emit() << ", " << right->Emit();
    }


    void Assembly::CreateMov(Value* left, Value* right)
    {
        CreateBinOp(left, right, "mov");
    }


    void Assembly::CreateAdd(Value* left, Value* right)
    {
        CreateBinOp(left, right, "add");
    }

    void Assembly::CreateSub(Value* left, Value* right)
    {
        CreateBinOp(left, right, "sub");
    }

    void Assembly::CreateMul(Value* left, Value* right)
    {
        CreateBinOp(left, right, "imul");
    }

    void Assembly::CreateDiv(Value*, Value*)
    {
        Diagnostics::Error("viper", "Unimplemented feature: Division");
    }


    void Assembly::CreateLeave()
    {
        _output << "\n\tleave";
    }

    void Assembly::CreateRet()
    {
        _output << "\n\tret";
    }

    void Assembly::VerifyArgs(Value* left, Value* right)
    {
        if(left->IsMemory() && right->IsMemory())
            Diagnostics::Error("viper", "Attempt to create binary operation with multiple memory references");
        if(left->IsImmediate())
            Diagnostics::Error("viper", "Attempt to create binary operation with immediate LHS");
        // TODO: Verify args
    }
}