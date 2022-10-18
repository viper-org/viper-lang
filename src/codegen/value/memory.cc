#include <codegen/value/memory.hh>

namespace Codegen
{
    MemoryValue::MemoryValue(int offset, bool isReference, std::shared_ptr<Type> type)
        :_offset(offset), _isReference(isReference), _type(type)
    {
    }

    MemoryValue::MemoryValue(MemoryValue* other, bool isReference)
        :_offset(other->_offset), _isReference(isReference), _type(other->_type)
    {
    }

    std::string MemoryValue::Emit(int)
    {
        return "[rbp-" + std::to_string(_offset) + "]";
    }

    bool MemoryValue::IsMemory()
    {
        return true;
    }

    void MemoryValue::Dispose()
    {
        return;
    }

    int MemoryValue::GetSize() const
    {
        return _type->GetPrimitiveSize();
    }
}