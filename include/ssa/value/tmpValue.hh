#ifndef VIPER_SSA_TMP_HH
#define VIPER_SSA_TMP_HH
#include <ssa/value/instruction/instruction.hh>
#include <memory>

namespace SSA
{
    class TempValue : public Value
    {
    public:
        void Print(std::ostream& stream, int indent) const override;
        std::string GetID() const override;

        Codegen::Value* Emit(Codegen::Assembly& assembly) override;

        void Dispose() override;

        TempValue(Module& module, const std::string& name);
    
    private:
        std::string _name;
    };
}

#endif