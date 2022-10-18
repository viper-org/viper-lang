#ifndef VIPER_SSA_VALUE_HH
#define VIPER_SSA_VALUE_HH
#include <ssa/module.hh>
#include <type/types.hh>
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
        virtual std::string GetID() const { return ""; }
        std::shared_ptr<Type> GetType() const { return _type; }

        virtual Codegen::Value* Emit(Codegen::Assembly& assembly) = 0;

        virtual void Dispose() { delete this; }
    protected:
        void SetType(std::shared_ptr<Type> newType) { _type = newType; }
        std::shared_ptr<Type> _type;
    private:
        Module& _module;
    };
}

#endif