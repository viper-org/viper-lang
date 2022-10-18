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

    enum class RegisterType
    {
        Integral,
        Floating,
    };

    class Register : public Value
    {
    public:
        Register(std::string_view id8, std::string_view id16, std::string_view id32, std::string_view id64, RegisterType type);

        std::string Emit(int bits) override;
        std::string_view GetID(int bits) const;

        static Register* AllocRegister(RegisterType type);
        static void FreeRegister(Register* reg);
        static Register* GetRegister(std::string_view id);

        bool IsRegister() override;

        void Dispose() override;

        int GetSize() const override;
    private:
        RegisterType _type;
        std::string_view _id8;
        std::string_view _id16;
        std::string_view _id32;
        std::string_view _id64;
    };
}

#endif