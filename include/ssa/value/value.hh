#ifndef VIPER_SSA_VALUE_HH
#define VIPER_SSA_VALUE_HH
#include <ssa/module.hh>
#include <codegen/assembly.hh>
#include <ostream>
#include <memory>

namespace SSA
{
    class Value
    {
    public:
        Value(Module& module) :_module(module) {  }
        virtual ~Value() {  }

        virtual void Print(std::ostream& stream, int indent) const = 0;

        virtual Codegen::Value* Emit(Codegen::Assembly& assembly) = 0;

        virtual void Dispose() { delete this; }
    private:
        Module& _module;
    };
}

#endif