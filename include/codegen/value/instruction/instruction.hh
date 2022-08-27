#ifndef VIPER_CODEGEN_INSTRUCTION_HH
#define VIPER_CODEGEN_INSTRUCTION_HH
#include <codegen/value/value.hh>

namespace Codegen
{
    class Instruction : public Value
    {
    public:
        enum Instructions
        {
            Add, Sub,
            Mul, Div,
        };
    protected:
        Instruction(const Module& module) :Value(module) {  }
    };
}

#endif