#ifndef VIPER_CODEGEN_MODULE_HH
#define VIPER_CODEGEN_MODULE_HH
#include <vector>
#include <string>
#include <map>

namespace Codegen
{
    class Value;
    class Global;
    class Function;
    class Module
    {
    public:
        Module(const std::string& id);

        const std::string& GetID() const;

        const std::string Generate();

        std::vector<Global*>& GetGlobals();
        Function* GetFunction(const std::string& name) const;
    private:
        std::vector<Global*> _globals;
        std::string _id;
    };
}

#endif