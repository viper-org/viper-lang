#include <codegen/assembly.hh>
#include <diagnostics.hh>
#include <unordered_map>

namespace Codegen
{
    using namespace std::literals;

    constexpr std::string_view GetOpSize(int bits)
    {
        switch(bits)
        {
            case 8:
                return "BYTE";
            case 16:
                return "WORD";
            case 32:
                return "DWORD";
            case 64:
                return "QWORD";
            default: throw;
                Diagnostics::Error("viper", "Invalid bits for operation size: " + std::to_string(bits));
        }
    }

    Assembly::Assembly()
    {
    }

    void Assembly::Emit(std::ostream& stream)
    {
        stream << _output.str() << "\n";
    }


    void Assembly::CreateGlobal(std::string_view ident)
    {
        _output << "\n\t[global " << ident << "]";
    }

    void Assembly::CreateLabel(std::string_view ident)
    {
        _output << "\n" << ident << ":";
    }

    void Assembly::CreateExtern(std::string_view ident)
    {
        _output << "\n\t[extern " << ident << "]";
    }


    void Assembly::CreateIndirect(Value* dest, Value* src)
    {
        int smallerSize = src->GetSize() > dest->GetSize() ? dest->GetSize() : src->GetSize();
        _output << "\n\tmov " << GetOpSize(smallerSize) << " " << dest->Emit(smallerSize) << ", [" << src->Emit(smallerSize) << "]";
    }


    void Assembly::CreateJmp(std::string_view label)
    {
        _output << "\n\tjmp " << label;
    }


    void Assembly::CreatePush(Value* operand)
    {
        _output << "\n\tpush " << GetOpSize(operand->GetSize()) << " " << operand->Emit(64);
    }
    

    void Assembly::CreateSet(CompareOperator op, Value* operand)
    {
        _output << "\n\tset";
        switch(op)
        {
            case CompareOperator::EQ:
                _output << "e";
                break;
            case CompareOperator::NE:
                _output << "ne";
                break;
            case CompareOperator::LT:
                _output << "l";
                break;
            case CompareOperator::GT:
                _output << "g";
                break;
            case CompareOperator::LE:
                _output << "le";
                break;
            case CompareOperator::GE:
                _output << "ge";
                break;
        }
        _output << " " << operand->Emit(8);
    }


    void Assembly::CreateBinOp(Value* left, Value* right, std::string_view op)
    {
        if(left == right)
            return;
        VerifyArgs(left, right);

        int smallerSize = left->GetSize() > right->GetSize() ? right->GetSize() : left->GetSize();
        if(op == "lea")
            smallerSize = 64;

        _output << "\n\t" << op << ' ' << GetOpSize(smallerSize) << ' ' << left->Emit(smallerSize) << ", " << right->Emit(smallerSize);
    }


    void Assembly::CreateMov(Value* dest, Value* src)
    {
        if(src->IsCompare())
        {
            CreateSet(static_cast<Compare*>(src)->GetOperator(), dest);
            return;
        }
        if(MemoryValue* mem; (mem = dynamic_cast<MemoryValue*>(src)) && !mem->IsReference())
            CreateBinOp(dest, src, "lea");
        else
            CreateBinOp(dest, src, "mov");
    }


    void Assembly::CreateAdd(Value* dest, Value* src)
    {
        CreateBinOp(dest, src, "add");
    }

    void Assembly::CreateSub(Value* dest, Value* src)
    {
        CreateBinOp(dest, src, "sub");
    }

    void Assembly::CreateMul(Value* dest, Value* src)
    {
        CreateBinOp(dest, src, "imul");
    }

    void Assembly::CreateDiv(Value*, Value*)
    {
        Diagnostics::Error("viper", "Unimplemented feature: Division");
    }


    void Assembly::CreateCmp(Value* left, Value* right)
    {
        CreateBinOp(left, right, "cmp");
    }


    void Assembly::CreateCall(std::string_view label)
    {
        _output << "\n\tcall " << label;
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
    }
}