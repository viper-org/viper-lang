#ifndef VIPER_CODEGEN_REGISTER_HH
#define VIPER_CODEGEN_REGISTER_HH
#include <codegen/value/value.hh>

namespace Codegen
{
    enum class Registers : int
    {
        RAX = 0,
        RCX,
        RDX,
        RBX,
        RSP,
        RBP,
        RSI,
        RDI,
        R8,
        R9,
        R10,
        R11,
        R12,
        R13,
        R14,
        R15,
        XMM0,
        XMM1,
        XMM2,
        XMM3,
        XMM4,
        XMM5,
        XMM6,
        XMM7,
        XMM8,
        XMM9,
        XMM10,
        XMM11,
        XMM12,
        XMM13,
        XMM14,
        XMM15,
    };

    class Register : public Value
    {
    public:
        Register(Registers id);

        std::string Emit() override;

        bool IsRegister() override;
    private:
        Registers _id;
    };
}

#endif