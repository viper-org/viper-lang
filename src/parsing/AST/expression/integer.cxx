#include <parsing/AST/expression/integer.hxx>

namespace Viper
{
    namespace Parsing
    {
        IntegerLiteral::IntegerLiteral(int value)
            :_value(value)
        {
        }

        void IntegerLiteral::Print(std::ostream& stream) const
        {
            stream << "<Integer-Literal>: " << _value;
        }
    }
}