#ifndef VIPER_SSA_MODULE_HH
#define VIPER_SSA_MODULE_HH
#include <string>
#include <vector>
#include <memory>

namespace SSA
{
    class Value;
    class Function;
    class Module
    {
    public:
        Module(const std::string& id);
        ~Module();

        int GetNextInstName();

        std::vector<Value*>& GetGlobals();
        Function* GetFunction(std::string_view name) const;
    private:
        std::vector<Value*> _globals;
        std::string _id;
        int _instNameCount;
    };
};

#endif