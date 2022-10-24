#include <ssa/value/instruction/binOp.hh>

namespace SSA
{
    BinOp::BinOp(Module& module, InstType type, Value* lhs, Value* rhs, const std::string& name)
        :Instruction(module), _name(new TempValue(module, name)), _lhs(lhs), _rhs(rhs)
    {
        _instType = type;
    }

    std::string_view InstTypeToString(Instruction::InstType type)
    {
        switch(type)
        {
            case Instruction::Add:
                return "add";
            case Instruction::Sub:
                return "sub";
            case Instruction::Div:
                return "div";
            case Instruction::Mul:
                return "mul";
            case Instruction::EQ:
                return "cmp eq";
            case Instruction::NE:
                return "cmp ne";
            case Instruction::LT:
                return "cmp lt";
            case Instruction::GT:
                return "cmp gt";
            default:
                return "";
        }
    }

    std::string BinOp::GetID() const
    {
        return _name->GetID();
    }

    void BinOp::Print(std::ostream& stream, int indent) const
    {
        _lhs->Print(stream, indent);
        _rhs->Print(stream, indent);

        _name->Print(stream, indent);
        stream << InstTypeToString(_instType) << " " << _lhs->GetID() << ", " << _rhs->GetID() << "\n";
    }

    Codegen::Value* BinOp::Emit(Codegen::Assembly& assembly)
    {
        Codegen::Value* lhs = _lhs->Emit(assembly);
        Codegen::Value* rhs = _rhs->Emit(assembly);

        if(!lhs->IsRegister())
        {
            Codegen::Register* reg = Codegen::Register::AllocRegister(Codegen::RegisterType::Integral);
            assembly.CreateMov(reg, lhs);
            lhs->Dispose();
            lhs = reg;
        }

        Codegen::Value* result = lhs;

        switch(_instType)
        {
            case Instruction::Add:
                assembly.CreateAdd(lhs, rhs);
                break;
            case Instruction::Sub:
                assembly.CreateSub(lhs, rhs);
                break;
            case Instruction::Mul:
                assembly.CreateMul(lhs, rhs);
                break;
            case Instruction::EQ:
            {
                assembly.CreateCmp(lhs, rhs);
                result = new Codegen::Compare(Codegen::CompareOperator::EQ);
                lhs->Dispose();
                break;
            }
            case Instruction::NE:
            {
                assembly.CreateCmp(lhs, rhs);
                result = new Codegen::Compare(Codegen::CompareOperator::NE);
                break;
            }
            case Instruction::LT:
            {
                assembly.CreateCmp(lhs, rhs);
                result = new Codegen::Compare(Codegen::CompareOperator::LT);
                break;
            }
            case Instruction::GT:
            {
                assembly.CreateCmp(lhs, rhs);
                result = new Codegen::Compare(Codegen::CompareOperator::GT);
                break;
            }
            default:
                break;
        }

        rhs->Dispose();

        return result;
    }

    void BinOp::Dispose()
    {
        _lhs->Dispose();
        _rhs->Dispose();
        _name->Dispose();
        delete this;
    }
}