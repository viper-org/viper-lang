#include <codegen/asm/register.hh>
#include <iostream>

namespace Codegen
{
    std::vector<std::pair<bool, Register*>> registers = { 
        { false, new Register("%rax") },
        { false, new Register("%rbx") },
        { false, new Register("%rcx") },
        { false, new Register("%rdx") },
        { false, new Register("%rdi") },
        { false, new Register("%rsi") },
    };
    Register::Register(const std::string& id)
        :_id(id)
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
        std::cerr << "Out of available registers!" << std::endl;
        std::exit(1);
    }

    Register* Register::FindRegister(const std::string& id)
    {
        for(std::pair<bool, Register*>& reg : registers)
        {
            if(reg.second->GetID() == id)
            {
                reg.first = true;
                return reg.second;
            }
        }
        std::cerr << "Unknown register '" + id + "'" << std::endl;
        std::exit(1);
    }

    void Register::FreeRegister(Register* reg)
    {
        for(std::pair<bool, Register*>& r : registers)
        {
            if(r.second == reg)
                r.first = false;
        }
    }

    std::string Register::GetID() const
    {
        return _id;
    }

    void Register::DestroyRegisters()
    {
        for(std::pair<bool, Register*>& reg : registers)
        {
            delete reg.second;
        }
    }
}