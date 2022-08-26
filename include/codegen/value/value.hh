#ifndef VIPER_CODEGEN_VALUE_HH
#define VIPER_CODEGEN_VALUE_HH
#include <codegen/module.hh>

namespace Codegen
{
    class Value
    {
    public:
        virtual ~Value() = default;
        virtual const std::string Generate() = 0;
    friend class Module;
    protected:
        Value(const Module& module) :_module(module) {  }
        const Module& _module;
    };
}

#endif