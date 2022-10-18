#ifndef VIPER_SSA_FUNCTION_HH
#define VIPER_SSA_FUNCTION_HH
#include <ssa/value/value.hh>
#include <ssa/value/basicBlock.hh>
#include <ssa/value/instruction/alloca.hh>
#include <memory>

namespace SSA
{
    class Function : public Value
    {
    public:
        static Function* Create(Module& module, const std::string& name);

        std::vector<BasicBlock*>& GetBasicBlockList();
        std::vector<AllocaInst*>& GetAllocaList();

        void Print(std::ostream& stream, int indent) const override;
        std::string GetID() const override;
        std::string_view GetName() const;

        Codegen::Value* Emit(Codegen::Assembly& assembly) override;

        void Dispose() override;

    protected:
        Function(Module& module, const std::string& name);

    private:
        std::string _name;
        int _totalAllocaOffset;
        std::vector<BasicBlock*> _basicBlockList;
        std::vector<AllocaInst*> _allocaList;

        void SortAllocas();
    };
}

#endif