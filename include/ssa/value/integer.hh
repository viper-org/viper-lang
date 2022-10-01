#ifndef VIPER_SSA_VALUE_INTEGER_HH
#define VIPER_SSA_VALUE_INTEGER_HH
#include <ssa/value.hh>

namespace SSA
{
    class IntegerLiteral : public Value
    {
    public:
        IntegerLiteral(long long value);

        void Print(std::ostream& stream, int indent) const override;

        std::unique_ptr<Codegen::Value> Emit(Codegen::Assembly& assembly) override;
    private:
        long long _value;
    };
}

#endif