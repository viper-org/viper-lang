#ifndef VIPER_CODEGEN_VALUE_HH
#define VIPER_CODEGEN_VALUE_HH
#include <codegen/module.hh>
#include <codegen/asm/register.hh>

namespace Codegen
{
    class Value
    {
    public:
        virtual ~Value() = default;
        virtual const std::pair<std::string, Register*> Generate(Register* reg = nullptr) = 0;
    friend class Module;
    protected:
        Value(const Module& module) :_module(module) {  }
        const Module& _module;
    };
}

#endif