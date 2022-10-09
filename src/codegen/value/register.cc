#include <codegen/value/register.hh>
#include <diagnostics.hh>
#include <array>

namespace Codegen
{
    using namespace std::literals;

    const std::array registers = {
        std::make_pair(false, new Register(""sv, ""sv, ""sv, "rax"sv,   RegisterType::Integral)),
        std::make_pair(false, new Register(""sv, ""sv, ""sv, "rcx"sv,   RegisterType::Integral)),
        std::make_pair(false, new Register(""sv, ""sv, ""sv, "rdx"sv,   RegisterType::Integral)),
        std::make_pair(false, new Register(""sv, ""sv, ""sv, "rbx"sv,   RegisterType::Integral)),
        std::make_pair(false, new Register(""sv, ""sv, ""sv, "rsp"sv,   RegisterType::Integral)),
        std::make_pair(false, new Register(""sv, ""sv, ""sv, "rbp"sv,   RegisterType::Integral)),
        std::make_pair(false, new Register(""sv, ""sv, ""sv, "rsi"sv,   RegisterType::Integral)),
        std::make_pair(false, new Register(""sv, ""sv, ""sv, "rdi"sv,   RegisterType::Integral)),
        std::make_pair(false, new Register(""sv, ""sv, ""sv, "r8"sv,    RegisterType::Integral)),
        std::make_pair(false, new Register(""sv, ""sv, ""sv, "r9"sv,    RegisterType::Integral)),
        std::make_pair(false, new Register(""sv, ""sv, ""sv, "r10"sv,   RegisterType::Integral)),
        std::make_pair(false, new Register(""sv, ""sv, ""sv, "r11"sv,   RegisterType::Integral)),
        std::make_pair(false, new Register(""sv, ""sv, ""sv, "r12"sv,   RegisterType::Integral)),
        std::make_pair(false, new Register(""sv, ""sv, ""sv, "r13"sv,   RegisterType::Integral)),
        std::make_pair(false, new Register(""sv, ""sv, ""sv, "r14"sv,   RegisterType::Integral)),
        std::make_pair(false, new Register(""sv, ""sv, ""sv, "r15"sv,   RegisterType::Integral)),
        std::make_pair(false, new Register(""sv, ""sv, ""sv, "xmm0"sv,  RegisterType::Floating)),
        std::make_pair(false, new Register(""sv, ""sv, ""sv, "xmm1"sv,  RegisterType::Floating)),
        std::make_pair(false, new Register(""sv, ""sv, ""sv, "xmm2"sv,  RegisterType::Floating)),
        std::make_pair(false, new Register(""sv, ""sv, ""sv, "xmm3"sv,  RegisterType::Floating)),
        std::make_pair(false, new Register(""sv, ""sv, ""sv, "xmm4"sv,  RegisterType::Floating)),
        std::make_pair(false, new Register(""sv, ""sv, ""sv, "xmm5"sv,  RegisterType::Floating)),
        std::make_pair(false, new Register(""sv, ""sv, ""sv, "xmm6"sv,  RegisterType::Floating)),
        std::make_pair(false, new Register(""sv, ""sv, ""sv, "xmm7"sv,  RegisterType::Floating)),
        std::make_pair(false, new Register(""sv, ""sv, ""sv, "xmm8"sv,  RegisterType::Floating)),
        std::make_pair(false, new Register(""sv, ""sv, ""sv, "xmm9"sv,  RegisterType::Floating)),
        std::make_pair(false, new Register(""sv, ""sv, ""sv, "xmm10"sv, RegisterType::Floating)),
        std::make_pair(false, new Register(""sv, ""sv, ""sv, "xmm11"sv, RegisterType::Floating)),
        std::make_pair(false, new Register(""sv, ""sv, ""sv, "xmm12"sv, RegisterType::Floating)),
        std::make_pair(false, new Register(""sv, ""sv, ""sv, "xmm13"sv, RegisterType::Floating)),
        std::make_pair(false, new Register(""sv, ""sv, ""sv, "xmm14"sv, RegisterType::Floating)),
        std::make_pair(false, new Register(""sv, ""sv, ""sv, "xmm15"sv, RegisterType::Floating)),
    };

    Register::Register(std::string_view id8, std::string_view id16, std::string_view id32, std::string_view id64, RegisterType type)
        :_type(type), _id8(id8), _id16(id16), _id32(id32), _id64(id64)
    {
    }

    Register* Register::AllocRegister(RegisterType type)
    {
        for(std::pair<bool, Register*> reg : registers)
        {
            if(!reg.first && type == reg.second->_type)
            {
                reg.first = true;
                return reg.second;
            }
        }
        Diagnostics::Error("viper", "Out of available registers!");
    }

    void Register::FreeRegister(Register* reg)
    {
        for(std::pair<bool, Register*> r : registers)
        {
            if(r.second == reg)
                r.first = false;
        }
    }

    Register* Register::GetRegister(std::string_view id)
    {
        for(std::pair<bool, Register*> reg : registers)
        {
            if(reg.second->GetID(64) == id)
            {
                reg.first = true;
                return reg.second;
            }
        }
        Diagnostics::Error("viper", "Unknown register '"s + id.data() + "'");
    }

    std::string Register::Emit()
    {
        return std::string(_id64.data());
    }

    std::string_view Register::GetID(int bits) const
    {
        switch(bits)
        {
            case 64:
                return _id64;
            case 32:
                return _id32;
            case 16:
                return _id16;
            case 8:
                return _id8;
            default:
                Diagnostics::Error("viper", "Invalid bits for register: " + std::to_string(bits));
        }
    }

    bool Register::IsRegister()
    {
        return true;
    }
}