#ifndef VIPER_SSA_INSTRUCTION_CALL_HH
#define VIPER_SSA_INSTRUCTION_CALL_HH
#include <ssa/value/instruction/instruction.hh>
#include <ssa/value/tmpValue.hh>
#include <memory>

namespace SSA
{
    class CallInst : public Instruction
    {
    friend class Builder;
    public:
        void Print(std::ostream& stream, int indent) const override;
        std::string GetID() const override;

        Codegen::Value* Emit(Codegen::Assembly& assembly) override;

        void Dispose() override;

    protected:
        CallInst(Module& module, Function* callee, const std::vector<Value*>& args, const std::string& name = "");
        
    private:
        TempValue* _name;
        Function* _callee;
        std::vector<Value*> _args;
    };
}

#endif