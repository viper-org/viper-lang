#ifndef VIPER_SSA_INSTRUCTION_HH
#define VIPER_SSA_INSTRUCTION_HH
#include <ssa/value.hh>

namespace SSA
{
    enum class InstType
    {
        Return,
    };

    class Instruction : public Value
    {
    public:
        virtual ~Instruction() {  }
    };
}

#endif