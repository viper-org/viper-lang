#include <codegen/value/instruction/binaryOperator.hh>

namespace Codegen
{
    BinaryOperator* BinaryOperator::Create(Value* lhs, Instruction::Instructions op, Value* rhs, const Module& module)
    {
        BinaryOperator* binOp = new BinaryOperator(lhs, op, rhs, module);

        return binOp;
    }

    BinaryOperator::BinaryOperator(Value* lhs, Instruction::Instructions op, Value* rhs, const Module& module)
        :Instruction(module), _lhs(lhs), _operator(op), _rhs(rhs)
    {
    }

    const std::pair<std::string, Register*> BinaryOperator::Generate(Register* reg)
    {
        std::pair<std::string, Register*> left  = _lhs->Generate();
        std::pair<std::string, Register*> right = _rhs->Generate();
        std::string result = left.first + right.first;

        switch(_operator)
        {
            case Instruction::Add:
                result += "\n\taddq ";
                break;
            case Instruction::Sub:
                result += "\n\tsubq ";
                break;
            case Instruction::Mul:
                result += "\n\timulq ";
                break;
            case Instruction::Div:
                throw; // TODO: Implement division
                break;
        }

        delete _lhs;
        delete _rhs;

        if(!reg)
        {
            if(left.second->GetID() == "%rax")
            {
                result += right.second->GetID() + ", " + left.second->GetID();
                Register::FreeRegister(right.second);
                reg = left.second;
            }
            else
            {
                result += left.second->GetID() + ", " + right.second->GetID();
                Register::FreeRegister(left.second);
                reg = right.second;
            }
        }
        else
        {
            result += right.second->GetID() + ", " + left.second->GetID();
            Register::FreeRegister(left.second);
            Register::FreeRegister(right.second);
        }

        return std::make_pair(result, reg);
    }
}