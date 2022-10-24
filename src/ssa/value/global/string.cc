#include <ssa/value/global/string.hh>

namespace SSA
{
    GlobalString::GlobalString(Module& module, const std::string& value)
        :Value(module), _value(value)
    {
    }

    GlobalString* GlobalString::Create(Module& module, const std::string& value)
    {
        GlobalString* str = new GlobalString(module, value);

        return str;
    }

    void GlobalString::Print(std::ostream&, int) const
    {
        // TODO: Add printing
    }

    std::string GlobalString::GetID() const
    {
        // TODO: Add ID
        return "";
    }

    Codegen::Value* GlobalString::Emit(Codegen::Assembly& assembly)
    {
        return assembly.CreateString(_value);
    }

    void GlobalString::Dispose()
    {
        delete this;
    }
}