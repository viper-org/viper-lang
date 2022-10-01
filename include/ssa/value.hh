#ifndef VIPER_SSA_VALUE_HH
#define VIPER_SSA_VALUE_HH
#include <ostream>

namespace SSA
{
    class Value
    {
    public:
        virtual ~Value() {  }

        virtual void Print(std::ostream& stream, int indent) const = 0;
    };
}

#endif