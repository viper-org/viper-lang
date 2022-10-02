#ifndef VIPER_SSA_VALUE_INTEGER_HH
#define VIPER_SSA_VALUE_INTEGER_HH
#include <ssa/value/value.hh>

namespace SSA
{
    class IntegerLiteral : public Value
    {
    public:
        IntegerLiteral(Module& module, long long value);

        void Print(std::ostream& stream, int indent) const override;

        long long GetValue() const;

        Codegen::Value* Emit(Codegen::Assembly& assembly) override;
    private:
        long long _value;
    };
}

#endif