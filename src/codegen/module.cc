#include <codegen/module.hh>
#include <codegen/value/value.hh>
#include <codegen/value/global/global.hh>
#include <codegen/value/global/function.hh>

namespace Codegen
{
    Module::Module(const std::string& id)
        :_id(id)
    {
    }

    Module::~Module()
    {
        for(Global* global : _globals)
            delete global;
    }

    const std::string& Module::GetID() const
    {
        return _id;
    }

    const std::string Module::Generate()
    {
        std::string result = "\t.file   \"" + _id;
        result += "\"\n\t.text";
        for(Global* global : _globals)
            result += global->Generate();
        
        result += "\n\t.section\t.note.GNU-stack,\"\",@progbits\n";

        return result;
    }

    std::vector<Global*>& Module::GetGlobals()
    {
        return _globals;
    }
}