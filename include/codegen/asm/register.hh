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
        std::string GetID(int bits) const;
    
        Register(const std::string& id64, const std::string& id32, const std::string& id16, const std::string& id8);
    
    private:
        std::string _id64;
        std::string _id32;
        std::string _id16;
        std::string _id8;
    };
}

#endif