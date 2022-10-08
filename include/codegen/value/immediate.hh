#ifndef VIPER_CODEGEN_IMMEDIATE_HH
#define VIPER_CODEGEN_IMMEDIATE_HH
#include <codegen/value/value.hh>

namespace Codegen
{
    class ImmediateValue : public Value
    {
    public:
        ImmediateValue(long long value);

        std::string Emit() override;

        bool IsImmediate() override;
        bool RequiresSize() override;
    private:
        long long _value;
    };
}

#endif