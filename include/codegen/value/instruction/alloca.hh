#ifndef VIPER_CODEGEN_ALLOCA_HH
#define VIPER_CODEGEN_ALLOCA_HH
#include <codegen/value/instruction/instruction.hh>

namespace Codegen
{
    class AllocaInst : public Instruction
    {
    friend class Builder;
    friend class Function;
    public:
        const std::pair<std::string, Register*> Generate(Register* reg = nullptr) override;
    protected:
        AllocaInst(Type* type, int offset, const Module& module);
    private:
        int _offset;
    };
}

#endif