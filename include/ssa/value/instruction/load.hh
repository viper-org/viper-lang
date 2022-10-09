
#ifndef VIPER_SSA_INSTRUCTION_LOAD_HH
#define VIPER_SSA_INSTRUCTION_LOAD_HH
#include <ssa/value/instruction/instruction.hh>
#include <ssa/value/tmpValue.hh>
#include <memory>

namespace SSA
{
    class LoadInst : public Instruction
    {
    friend class Builder;
    public:
        void Print(std::ostream& stream, int indent) const override;
        std::string GetID() const override;

        Codegen::Value* Emit(Codegen::Assembly& assembly) override;

        void Dispose() override;

    protected:
        LoadInst(Module& module, Value* ptr, const std::string& name = "");
        
    private:
        TempValue* _name;
        Value* _ptr;
    };
}

#endif