#ifndef VIPER_CODEGEN_LOAD_HH
#define VIPER_CODEGEN_LOAD_HH
#include <codegen/value/instruction/instruction.hh>

namespace Codegen
{
    class LoadInst : public Instruction
    {
    friend class Builder;
    public:
        const std::pair<std::string, Register*> Generate(Register* reg = nullptr) override;
    protected:
        LoadInst(Value* ptr, const Module& module);
    private:
        Value* _ptr;
    };
}

#endif