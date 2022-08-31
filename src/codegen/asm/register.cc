#include <codegen/asm/register.hh>
#include <diagnostics.hh>

namespace Codegen
{
    std::vector<std::pair<bool, Register*>> registers = { 
        { false, new Register("%rax", "%eax", "%ax", "%al") },
        { false, new Register("%rbx", "%ebx", "%bx", "%bl") },
        { false, new Register("%rcx", "%ecx", "%cx", "%cl") },
        { false, new Register("%rdx", "%edx", "%dx", "%dl") },
        { false, new Register("%rdi", "%edi", "%di", "%dil") },
        { false, new Register("%rsi", "%esi", "%si", "%sil") },
    };
    Register::Register(const std::string& id64, const std::string& id32, const std::string& id16, const std::string& id8)
        :_id64(id64), _id32(id32), _id16(id16), _id8(id8)
    {
    }

    Register* Register::GetRegister()
    {
        for(std::pair<bool, Register*>& reg : registers)
        {
            if(!reg.first)
            {
                reg.first = true;
                return reg.second;
            }
        }
        Diagnostics::Error("viper", "Out of available registers!");
    }

    Register* Register::FindRegister(const std::string& id)
    {
        for(std::pair<bool, Register*>& reg : registers)
        {
            if(reg.second->GetID(64) == id)
            {
                reg.first = true;
                return reg.second;
            }
        }
        Diagnostics::Error("viper", "Unknown register '" + id + "'");
    }

    void Register::FreeRegister(Register* reg)
    {
        for(std::pair<bool, Register*>& r : registers)
        {
            if(r.second == reg)
                r.first = false;
        }
    }

    std::string Register::GetID(int bits) const
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
                Diagnostics::Error("viper", "No registers with " + std::to_string(bits) + " bits");
        }
    }

    void Register::DestroyRegisters()
    {
        for(std::pair<bool, Register*>& reg : registers)
        {
            delete reg.second;
        }
    }
}