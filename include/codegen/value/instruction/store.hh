#ifndef VIPER_CODEGEN_STORE_HH
#define VIPER_CODEGEN_STORE_HH
#include <codegen/value/instruction/instruction.hh>

namespace Codegen
{
    class StoreInst : public Instruction
    {
    friend class Builder;
    public:
        const std::pair<std::string, Register*> Generate(Register* reg = nullptr) override;
    protected:
        StoreInst(Value* value, Value* ptr, const Module& module);
    private:
        Value* _value;
        Value* _ptr;
    };
}

#endif