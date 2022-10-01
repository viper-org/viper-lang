#include <codegen/value/register.hh>

namespace Codegen
{
    const std::string registerNames[] = {
        "rax",
        "rcx",
        "rdx",
        "rbx",
        "rsp",
        "rbp",
        "rsi",
        "rdi",
        "r8",
        "r9",
        "r10",
        "r11",
        "r12",
        "r13",
        "r14",
        "r15",
        "xmm0",
        "xmm1",
        "xmm2",
        "xmm3",
        "xmm4",
        "xmm5",
        "xmm6",
        "xmm7",
        "xmm8",
        "xmm9",
        "xmm10",
        "xmm11",
        "xmm12",
        "xmm13",
        "xmm14",
        "xmm15",
    };

    Register::Register(Registers id)
        :_id(id)
    {
    }

    std::string Register::Emit()
    {
        return registerNames[static_cast<int>(_id)];
    }

    bool Register::IsRegister()
    {
        return true;
    }
}