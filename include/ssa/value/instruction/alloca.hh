#ifndef VIPER_SSA_INSTRUCTION_ALLOCA_HH
#define VIPER_SSA_INSTRUCTION_ALLOCA_HH
#include <ssa/value/instruction/instruction.hh>
#include <memory>

namespace SSA
{
    class AllocaInst : public Instruction
    {
    public:
        AllocaInst(Module& module, const std::string& name);

        void Print(std::ostream& stream, int indent) const override;

        Codegen::Value* Emit(Codegen::Assembly& assembly) override;

        void Dispose() override;
    private:
        std::string _name;
    };
}

#endif