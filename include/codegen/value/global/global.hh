#ifndef VIPER_CODEGEN_GLOBAL_HH
#define VIPER_CODEGEN_GLOBAL_HH
#include <codegen/value/value.hh>

namespace Codegen
{
    class Global : public Value
    {
    protected:
        Global(const Module& module) :Value(module) {  }
    };
}

#endif