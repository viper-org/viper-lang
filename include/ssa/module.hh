#ifndef VIPER_SSA_MODULE_HH
#define VIPER_SSA_MODULE_HH
#include <string>
#include <vector>
#include <memory>

namespace SSA
{
    class Module
    {
    public:
        Module(const std::string& id);

        int GetNextInstName();
    private:
        std::string _id;
        int _instNameCount;
    };
};

#endif