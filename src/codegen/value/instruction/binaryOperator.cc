#include <codegen/value/instruction/binaryOperator.hh>
#include <iostream>

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
                result += "\n\tadd";
                break;
            case Instruction::Sub:
                result += "\n\tsub";
                break;
            case Instruction::Mul:
                result += "\n\timul";
                break;
            case Instruction::Div:
                throw; // TODO: Implement division
                break;
        }
        result += _type->GetSuffix() + ' ';

        if(!reg)
        {
            if(left.second->GetID(64) == "%rax")
            {
                result += right.second->GetID(_type->GetSize()) + ", " + left.second->GetID(_type->GetSize());
                Register::FreeRegister(right.second);
                reg = left.second;
            }
            else
            {
                result += left.second->GetID(_type->GetSize()) + ", " + right.second->GetID(_type->GetSize());
                Register::FreeRegister(left.second);
                reg = right.second;
            }
        }
        else
        {
            result += right.second->GetID(_lhs->GetType()->GetSize()) + ", " + left.second->GetID(_lhs->GetType()->GetSize());
            Register::FreeRegister(left.second);
            Register::FreeRegister(right.second);
        }

        delete _lhs;
        delete _rhs;

        return std::make_pair(result, reg);
    }
}