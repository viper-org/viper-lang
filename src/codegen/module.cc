#include <codegen/module.hh>
#include <codegen/value/value.hh>
#include <codegen/value/global/global.hh>
#include <codegen/value/global/function.hh>
#include <codegen/value/instruction/alloca.hh>
#include <algorithm>

namespace Codegen
{
    const std::string& Module::GetID() const
    {
        return _id;
    }

    const std::string Module::Generate()
    {
        std::string result = "\t.file   \"" + _id;
        result += "\"\n\t.text";
        for(Global* global : _globals)
            result += global->Generate().first;

        result += "\n\t.section\t.note.GNU-stack,\"\",@progbits";

        for(Global* global : _globals)
            delete global;
        
        Register::DestroyRegisters();

        return result;
    }

    std::vector<Global*>& Module::GetGlobals()
    {
        return _globals;
    }

    Function* Module::GetFunction(const std::string& name) const
    {
        for(Global* global : _globals)
        {
            Function* func = dynamic_cast<Function*>(global);
            if(func)
                if(func->GetName() == name)
                    return func;
        }
        return nullptr;
    }
}