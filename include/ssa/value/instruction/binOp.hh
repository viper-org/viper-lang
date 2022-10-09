#ifndef VIPER_SSA_INSTRUCTION_BINOP_HH
#define VIPER_SSA_INSTRUCTION_BINOP_HH
#include <ssa/value/instruction/instruction.hh>
#include <ssa/value/tmpValue.hh>
#include <memory>

namespace SSA
{
    class BinOp : public Instruction
    {
    friend class Builder;
    public:
        void Print(std::ostream& stream, int indent) const override;
        std::string GetID() const override;

        Codegen::Value* Emit(Codegen::Assembly& assembly) override;

        void Dispose() override;

    protected:
        BinOp(Module& module, InstType type, Value* lhs, Value* rhs, const std::string& name = "");
        
    private:
        TempValue* _name;
        Value* _lhs;
        Value* _rhs;
    };
}

#endif