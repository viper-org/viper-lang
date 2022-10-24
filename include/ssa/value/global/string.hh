#ifndef VIPER_SSA_STRING_HH
#define VIPER_SSA_STRING_HH
#include <ssa/value/value.hh>
#include <ssa/value/basicBlock.hh>
#include <ssa/value/instruction/alloca.hh>
#include <memory>

namespace SSA
{
    class GlobalString : public Value
    {
    public:
        static GlobalString* Create(Module& module, const std::string& value);

        void Print(std::ostream& stream, int indent) const override;
        std::string GetID() const override;

        Codegen::Value* Emit(Codegen::Assembly& assembly) override;

        void Dispose() override;

    protected:
        GlobalString(Module& module, const std::string& value);

    private:
        std::string _value;
    };
}

#endif