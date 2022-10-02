#ifndef VIPER_SSA_FUNCTION_HH
#define VIPER_SSA_FUNCTION_HH
#include <ssa/value/value.hh>
#include <ssa/value/basicBlock.hh>
#include <memory>

namespace SSA
{
    class Function : public Value
    {
    public:
        Function(Module& module, const std::string& name);

        std::vector<BasicBlock*>& GetBasicBlockList();

        void Print(std::ostream& stream, int indent) const override;

        Codegen::Value* Emit(Codegen::Assembly& assembly) override;

        void Dispose() override;
    private:
        std::string _name;
        std::vector<BasicBlock*> _basicBlockList;
    };
}

#endif