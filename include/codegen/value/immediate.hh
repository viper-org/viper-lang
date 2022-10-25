#ifndef VIPER_CODEGEN_IMMEDIATE_HH
#define VIPER_CODEGEN_IMMEDIATE_HH
#include <codegen/value/value.hh>
#include <type/types.hh>

namespace Codegen
{
    class ImmediateValue : public Value
    {
    public:
        ImmediateValue(long long value, std::shared_ptr<Type> type);

        std::string Emit(int bits) override;

        bool IsImmediate() override;

        int GetSize() const override;
        long long GetValue() const;
    private:
        long long _value;
        std::shared_ptr<Type> _type;
    };
}

#endif