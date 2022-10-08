#ifndef VIPER_CODEGEN_VALUE_HH
#define VIPER_CODEGEN_VALUE_HH
#include <string>

namespace Codegen
{
    class Value
    {
    public:
        virtual ~Value() {  }

        virtual std::string Emit() = 0;

        virtual bool IsImmediate() { return false; }
        virtual bool IsRegister() { return false; }
        virtual bool IsMemory() { return false; }
    };
}

#endif