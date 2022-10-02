#ifndef VIPER_SSA_INSTRUCTION_STORE_HH
#define VIPER_SSA_INSTRUCTION_STORE_HH
#include <ssa/value/instruction/instruction.hh>
#include <memory>

namespace SSA
{
    class StoreInst : public Instruction
    {
    friend class Builder;
    public:
        void Print(std::ostream& stream, int indent) const override;

        Codegen::Value* Emit(Codegen::Assembly& assembly) override;

        void Dispose() override;

    protected:
        StoreInst(Module& module, Value* ptr, Value* value);
        
    private:
        Value* _ptr;
        Value* _value;
    };
}

#endif