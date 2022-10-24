#ifndef VIPER_CODEGEN_MEMORY_HH
#define VIPER_CODEGEN_MEMORY_HH
#include <codegen/value/value.hh>
#include <type/types.hh>

namespace Codegen
{
    class MemoryValue : public Value
    {
    public:
        MemoryValue(int offset, bool isReference, std::shared_ptr<Type> type);
        MemoryValue(MemoryValue* other, bool isReference);

        std::string Emit(int bits) override;

        bool IsMemory() override;
        bool IsReference() const;

        void Dispose() override;

        int GetSize() const override;
    private:
        int _offset;
        bool _isReference;
        std::shared_ptr<Type> _type;
    };
}

#endif