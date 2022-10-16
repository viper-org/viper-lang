#ifndef VIPER_SSA_INSTRUCTION_ALLOCA_HH
#define VIPER_SSA_INSTRUCTION_ALLOCA_HH
#include <ssa/value/instruction/instruction.hh>
#include <ssa/value/tmpValue.hh>
#include <memory>

namespace SSA
{
    class AllocaInst : public Instruction
    {
    friend class Builder;
    friend class Function;
    public:
        void Print(std::ostream& stream, int indent) const override;
        std::string GetID() const override;

        Codegen::Value* Emit(Codegen::Assembly& assembly) override;

        void Dispose() override;

    protected:
        AllocaInst(Module& module, const std::string& name = "");
    
    private:
        TempValue* _name;
        Codegen::Value* _memory;
        int _offset;
    };
}

#endif