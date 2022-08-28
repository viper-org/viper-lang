#ifndef VIPER_CODEGEN_REGISTER_HH
#define VIPER_CODEGEN_REGISTER_HH
#include <string>
#include <vector>

namespace Codegen
{
    class Register
    {
    public:
        static Register* GetRegister();
        static Register* FindRegister(const std::string& id);
        static void FreeRegister(Register* reg);
        static void DestroyRegisters();
        std::string GetID() const;
    
        Register(const std::string& id);
    
    private:
        std::string _id;
    };
}

#endif