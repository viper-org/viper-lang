#ifndef VIPER_SSA_INSTRUCTION_RETURN_HH
#define VIPER_SSA_INSTRUCTION_RETURN_HH
#include <ssa/value/instruction/instruction.hh>
#include <memory>

namespace SSA
{
    class RetInst : public Instruction
    {
    friend class Builder;
    public:
        void Print(std::ostream& stream, int indent) const override;

        Codegen::Value* Emit(Codegen::Assembly& assembly) override;

        void Dispose() override;
        
    protected:
        RetInst(Module& module, Value* value);
        
    private:
        Value* _value;
    };
}

#endif