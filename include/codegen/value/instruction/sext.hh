#ifndef VIPER_CODEGEN_SEXT_HH
#define VIPER_CODEGEN_SEXT_HH
#include <codegen/value/instruction/instruction.hh>

namespace Codegen
{
    class SExtInst : public Instruction
    {
    friend class Builder;
    public:
        const std::pair<std::string, Register*> Generate(Register* reg = nullptr) override;
    protected:
        SExtInst(Value* value, Type* type, const Module& module);
    private:
        Value* _value;
        Type* _dstType;
    };
}

#endif