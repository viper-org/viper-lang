#ifndef VIPER_SSA_INSTRUCTION_RETURN_HH
#define VIPER_SSA_INSTRUCTION_RETURN_HH
#include <ssa/value/instruction/instruction.hh>
#include <memory>

namespace SSA
{
    class Return : public Instruction
    {
    public:
        Return(Module& module, Value* value);

        void Print(std::ostream& stream, int indent) const override;

        Codegen::Value* Emit(Codegen::Assembly& assembly) override;

        void Dispose() override;
    private:
        Value* _value;
    };
}

#endif