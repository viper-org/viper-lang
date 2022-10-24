#ifndef VIPER_SSA_INSTRUCTION_HH
#define VIPER_SSA_INSTRUCTION_HH
#include <ssa/value/value.hh>

namespace SSA
{
    class Instruction : public Value
    {
    public:
        enum InstType
        {
            Ret,

            Alloca,
            Load,
            Store,

            Call,

            Add,
            Sub,
            Mul,
            Div,
        };

        Instruction(Module& module) : Value(module) {  }
        virtual ~Instruction() {  }

        InstType GetInstType() const { return _instType; }
    protected:
        InstType _instType;
    };
}

#endif