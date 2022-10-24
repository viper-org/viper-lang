#ifndef VIPER_CODEGEN_VALUE_HH
#define VIPER_CODEGEN_VALUE_HH
#include <string>

namespace Codegen
{
    class Value
    {
    public:
        virtual ~Value() {  }

        virtual std::string Emit(int bits) = 0;

        virtual bool IsImmediate() { return false; }
        virtual bool IsRegister() { return false; }
        virtual bool IsMemory() { return false; }
        virtual bool IsCompare() { return false; }

        virtual bool RequiresSize() { return false; }

        virtual void Dispose() { delete this; }

        virtual int GetSize() const = 0;
    };
}

#endif