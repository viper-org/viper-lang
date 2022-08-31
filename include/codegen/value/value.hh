#ifndef VIPER_CODEGEN_VALUE_HH
#define VIPER_CODEGEN_VALUE_HH
#include <codegen/module.hh>
#include <codegen/asm/register.hh>
#include <type/types.hh>

namespace Codegen
{
    class Builder;
    class Value
    {
    public:
        virtual ~Value() = default;
        virtual const std::pair<std::string, Register*> Generate(Register* reg = nullptr) = 0;
        Type* GetType() const { return _type; }
        void SetType(Type* type) { _type = type; }
    friend class Module;
    protected:
        Value(const Module& module) :_module(module) {  }
        Type* _type;
        const Module& _module;
    };
}

#endif