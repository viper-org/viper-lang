#include <codegen/value/memory.hh>

namespace Codegen
{
    MemoryValue::MemoryValue(int offset, bool isReference)
        :_offset(offset), _isReference(isReference)
    {
    }

    MemoryValue::MemoryValue(MemoryValue* other, bool isReference)
        :_offset(other->_offset), _isReference(isReference)
    {
    }

    std::string MemoryValue::Emit()
    {
        return "[rbp-" + std::to_string(_offset) + "]";
    }

    bool MemoryValue::IsMemory()
    {
        return true;
    }
}