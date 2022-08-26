#ifndef VIPER_CODEGEN_MODULE_HH
#define VIPER_CODEGEN_MODULE_HH
#include <vector>
#include <string>

namespace Codegen
{
    class Value;
    class Global;
    class Module
    {
    public:
        Module(const std::string& id);
        ~Module();

        const std::string& GetID() const;

        const std::string Generate();

        std::vector<Global*>& GetGlobals();
    private:
        std::vector<Global*> _globals;
        std::string _id;
    };
}

#endif