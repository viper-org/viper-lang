#include <ssa/module.hh>
#include <ssa/value/global/function.hh>

namespace SSA
{
    Module::Module(const std::string& id)
        :_id(id), _instNameCount(0)
    {
    }

    Module::~Module()
    {
        for(Value* global : _globals)
            global->Dispose();
    }

    int Module::GetNextInstName()
    {
        return _instNameCount++;
    }

    std::vector<Value*>& Module::GetGlobals()
    {
        return _globals;
    }

    Function* Module::GetFunction(std::string_view name) const
    {
        for(Value* global : _globals)
        {
            if(Function* func = dynamic_cast<Function*>(global))
                if(func->GetName() == name)
                    return func;
        }
        return nullptr;
    }
}