#ifndef VIPER_CODEGEN_BUILDER_HH
#define VIPER_CODEGEN_BUILDER_HH
#include <codegen/module.hh>
#include <codegen/value/value.hh>
#include <codegen/value/global/global.hh>
#include <codegen/value/global/function.hh>
#include <codegen/value/constant/intLiteral.hh>

namespace Codegen
{
    class Builder
    {
    public:
        Builder(const Module& module);

        void SetInsertFunction(Function* insertFunction);
        Value* CreateIntLiteral(long long value);
    private:
        const Module& _module;
        Function* _insertFunction;
    };
}

#endif