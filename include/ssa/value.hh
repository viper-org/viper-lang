#ifndef VIPER_SSA_VALUE_HH
#define VIPER_SSA_VALUE_HH
#include <codegen/assembly.hh>
#include <ostream>
#include <memory>

namespace SSA
{
    class Value
    {
    public:
        virtual ~Value() {  }

        virtual void Print(std::ostream& stream, int indent) const = 0;

        virtual std::unique_ptr<Codegen::Value> Emit(Codegen::Assembly& assembly) = 0;
    };
}

#endif