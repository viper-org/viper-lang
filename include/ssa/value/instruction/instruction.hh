#ifndef VIPER_SSA_INSTRUCTION_HH
#define VIPER_SSA_INSTRUCTION_HH
#include <ssa/value/value.hh>

namespace SSA
{
    enum class InstType
    {
        Ret,
        Alloca,
    };

    class Instruction : public Value
    {
    public:
        Instruction(Module& module) : Value(module) {  }
        virtual ~Instruction() {  }

        InstType GetInstType() const { return _instType; }
    protected:
        InstType _instType;
    };
}

#endif