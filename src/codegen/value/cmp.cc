#include <codegen/value/cmp.hh>

namespace Codegen
{
    Compare::Compare(CompareOperator op)
        :_operator(op)
    {
    }

    std::string Compare::Emit(int)
    {
        return "";
    }

    bool Compare::IsCompare()
    {
        return true;
    }

    CompareOperator Compare::GetOperator() const
    {
        return _operator;
    }

    int Compare::GetSize() const
    {
        return 1;
    }

    void Compare::Dispose()
    {
        delete this;
    }
}