#ifndef VIPER_CODEGEN_RET_HH
#define VIPER_CODEGEN_RET_HH
#include <codegen/value/value.hh>

namespace Codegen
{
    class RetVal : public Value
    {
    public:
        RetVal(Value* value, const Module& module);
        const std::string Generate() override;
    private:
        Value* _value;
    };
}

#endif