#ifndef VIPER_SSA_BASIC_BLOCK_HH
#define VIPER_SSA_BASIC_BLOCK_HH
#include <ssa/value/value.hh>
#include <ssa/value/instruction/instruction.hh>
#include <vector>
#include <memory>

namespace SSA
{
    class Function;
    class BasicBlock : public Value
    {
    public:
        static BasicBlock* Create(Module& module, Function* parent, const std::string& name = "");

        Function* GetParent() const;
        std::vector<Instruction*>& GetInstList();

        void Print(std::ostream& stream, int indent) const override;

        Codegen::Value* Emit(Codegen::Assembly& assembly) override;

        void Dispose() override;
    protected:
        BasicBlock(Module& module, Function* parent, const std::string& name = "");
    private:
        std::string _name;
        Function* _parent;
        std::vector<Instruction*> _instList;
    };
}

#endif