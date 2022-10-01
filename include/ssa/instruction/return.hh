#ifndef VIPER_SSA_INSTRUCTION_RETURN_HH
#define VIPER_SSA_INSTRUCTION_RETURN_HH
#include <ssa/instruction/instruction.hh>
#include <memory>

namespace SSA
{
    class Return : public Instruction
    {
    public:
        Return(std::unique_ptr<Value>& value);

        void Print(std::ostream& stream, int indent) const override;

        std::unique_ptr<Codegen::Value> Emit(Codegen::Assembly& assembly) override;
    private:
        std::unique_ptr<Value> _value;
    };
}

#endif