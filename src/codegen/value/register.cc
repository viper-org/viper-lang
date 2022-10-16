#include <codegen/value/register.hh>
#include <diagnostics.hh>
#include <array>

namespace Codegen
{
    using namespace std::literals;

    std::array registers = {
        std::make_pair(false, new Register("al"sv,   "ax"sv,   "eax"sv,  "rax"sv,   RegisterType::Integral)),
        std::make_pair(false, new Register("cl"sv,   "cx"sv,   "ecx"sv,  "rcx"sv,   RegisterType::Integral)),
        std::make_pair(false, new Register("dl"sv,   "dx"sv,   "edx"sv,  "rdx"sv,   RegisterType::Integral)),
        std::make_pair(false, new Register("bl"sv,   "bx"sv,   "ebx"sv,  "rbx"sv,   RegisterType::Integral)),
        std::make_pair(false, new Register("spl"sv,  "sp"sv,   "esp"sv,  "rsp"sv,   RegisterType::Integral)),
        std::make_pair(false, new Register("bpl"sv,  "bp"sv,   "ebp"sv,  "rbp"sv,   RegisterType::Integral)),
        std::make_pair(false, new Register("sil"sv,  "si"sv,   "esi"sv,  "rsi"sv,   RegisterType::Integral)),
        std::make_pair(false, new Register("dil"sv,  "di"sv,   "edi"sv,  "rdi"sv,   RegisterType::Integral)),
        std::make_pair(false, new Register("r8b"sv,  "r8w"sv,  "r8d"sv,  "r8"sv,    RegisterType::Integral)),
        std::make_pair(false, new Register("r9b"sv,  "r9w"sv,  "r9d"sv,  "r9"sv,    RegisterType::Integral)),
        std::make_pair(false, new Register("r10b"sv, "r10w"sv, "r10d"sv, "r10"sv,   RegisterType::Integral)),
        std::make_pair(false, new Register("r11b"sv, "r11w"sv, "r11d"sv, "r11"sv,   RegisterType::Integral)),
        std::make_pair(false, new Register("r12b"sv, "r12w"sv, "r12d"sv, "r12"sv,   RegisterType::Integral)),
        std::make_pair(false, new Register("r13b"sv, "r13w"sv, "r13d"sv, "r13"sv,   RegisterType::Integral)),
        std::make_pair(false, new Register("r14b"sv, "r14w"sv, "r14d"sv, "r14"sv,   RegisterType::Integral)),
        std::make_pair(false, new Register("r15b"sv, "r15w"sv, "r15d"sv, "r15"sv,   RegisterType::Integral)),
        std::make_pair(false, new Register("xmm0"sv, "xmm0"sv, "xmm0"sv, "xmm0"sv,  RegisterType::Floating)),
        std::make_pair(false, new Register("xmm1"sv, "xmm1"sv, "xmm1"sv, "xmm1"sv,  RegisterType::Floating)),
        std::make_pair(false, new Register("xmm2"sv, "xmm2"sv, "xmm2"sv, "xmm2"sv,  RegisterType::Floating)),
        std::make_pair(false, new Register("xmm3"sv, "xmm3"sv, "xmm3"sv, "xmm3"sv,  RegisterType::Floating)),
        std::make_pair(false, new Register("xmm4"sv, "xmm4"sv, "xmm4"sv, "xmm4"sv,  RegisterType::Floating)),
        std::make_pair(false, new Register("xmm5"sv, "xmm5"sv, "xmm5"sv, "xmm5"sv,  RegisterType::Floating)),
        std::make_pair(false, new Register("xmm6"sv, "xmm6"sv, "xmm6"sv, "xmm6"sv,  RegisterType::Floating)),
        std::make_pair(false, new Register("xmm7"sv, "xmm7"sv, "xmm7"sv, "xmm7"sv,  RegisterType::Floating)),
        std::make_pair(false, new Register("xmm8"sv, "xmm8"sv, "xmm8"sv, "xmm8"sv,  RegisterType::Floating)),
        std::make_pair(false, new Register("xmm9"sv, "xmm9"sv, "xmm9"sv, "xmm9"sv,  RegisterType::Floating)),
        std::make_pair(false, new Register("xmm10"sv, "xmm10"sv, "xmm10"sv, "xmm10"sv, RegisterType::Floating)),
        std::make_pair(false, new Register("xmm11"sv, "xmm11"sv, "xmm11"sv, "xmm11"sv, RegisterType::Floating)),
        std::make_pair(false, new Register("xmm12"sv, "xmm12"sv, "xmm12"sv, "xmm12"sv, RegisterType::Floating)),
        std::make_pair(false, new Register("xmm13"sv, "xmm13"sv, "xmm13"sv, "xmm13"sv, RegisterType::Floating)),
        std::make_pair(false, new Register("xmm14"sv, "xmm14"sv, "xmm14"sv, "xmm14"sv, RegisterType::Floating)),
        std::make_pair(false, new Register("xmm15"sv, "xmm15"sv, "xmm15"sv, "xmm15"sv, RegisterType::Floating)),
    };

    Register::Register(std::string_view id8, std::string_view id16, std::string_view id32, std::string_view id64, RegisterType type)
        :_type(type), _id8(id8), _id16(id16), _id32(id32), _id64(id64)
    {
    }

    Register* Register::AllocRegister(RegisterType type)
    {
        for(std::pair<bool, Register*>& reg : registers)
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
        for(std::pair<bool, Register*>& r : registers)
        {
            if(r.second == reg)
                r.first = false;
        }
    }

    Register* Register::GetRegister(std::string_view id)
    {
        for(std::pair<bool, Register*>& reg : registers)
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
        return std::string(_id32.data());
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

    void Register::Dispose()
    {
        FreeRegister(this);
    }
}