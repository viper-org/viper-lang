
#ifndef VIPER_SSA_INSTRUCTION_BRANCH_HH
#define VIPER_SSA_INSTRUCTION_BRANCH_HH
#include <ssa/value/instruction/instruction.hh>
#include <ssa/value/basicBlock.hh>

namespace SSA
{
    class BranchInst : public Instruction
    {
    friend class Builder;
    public:
        void Print(std::ostream& stream, int indent) const override;
        std::string GetID() const override;

        Codegen::Value* Emit(Codegen::Assembly& assembly) override;

        void Dispose() override;

    protected:
        BranchInst(Module& module, Value* cond, BasicBlock* trueBranch, BasicBlock* falseBranch);
        BranchInst(Module& module, BasicBlock* branch);
        
    private:
        Value* _cond;
        BasicBlock* _true;
        BasicBlock* _false;
    };
}

#endif