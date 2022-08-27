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

    const std::string BinaryOperator::Generate()
    {
        std::string result = _lhs->Generate();
        result += _rhs->Generate();
        result += "\n\tpopq %rax\n\tpopq %rdx";
        switch(_operator)
        {
            case Instruction::Add:
                result += "\n\taddl %edx, %eax";
                break;
            case Instruction::Sub:
                result += "\n\tsubl %edx, %eax";
                break;
            case Instruction::Mul:
                result += "\n\timull %edx, %eax";
                break;
            case Instruction::Div:
                throw; // TODO: Implement division
                break;
        }

        delete _lhs;
        delete _rhs;

        return result + "\n\tpushq %rax";
    }
}