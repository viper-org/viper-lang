#ifndef VIPER_CODEGEN_MEMORY_HH
#define VIPER_CODEGEN_MEMORY_HH
#include <codegen/value/value.hh>

namespace Codegen
{
    class MemoryValue : public Value
    {
    public:
        MemoryValue(int offset, bool isReference);
        MemoryValue(MemoryValue* other, bool isReference);

        std::string Emit() override;

        bool IsMemory() override;

        void Dispose() override;
    private:
        int _offset;
        bool _isReference;
    };
}

#endif