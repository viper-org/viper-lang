#ifndef VIPER_CODEGEN_BINARY_HH
#define VIPER_CODEGEN_BINARY_HH
#include <codegen/value/instruction/instruction.hh>

namespace Codegen
{
    class BinaryOperator : public Instruction
    {
    public:
        static BinaryOperator* Create(Value* lhs, Instruction::Instructions op, Value* rhs, const Module& module);
        const std::pair<std::string, Register*> Generate(Register* reg = nullptr) override;
    protected:
        BinaryOperator(Value* lhs, Instruction::Instructions op, Value* rhs, const Module& module);
    private:
        Value* _lhs;
        Instruction::Instructions _operator;
        Value* _rhs;
    };
}

#endif