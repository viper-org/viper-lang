#include <ssa/instruction/return.hh>

namespace SSA
{
    Return::Return(std::unique_ptr<Value>& value)
        :_value(std::move(value))
    {
    }

    void Return::Print(std::ostream& stream, int indent) const
    {
        stream << std::string(indent, ' ') << "ret ";
        if(_value)
            _value->Print(stream, indent);
        else
            stream << "void";
        
        stream << '\n';
    }
}