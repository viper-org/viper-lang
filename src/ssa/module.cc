#include <ssa/module.hh>

namespace SSA
{
    Module::Module(const std::string& id)
        :_id(id), _instNameCount(0)
    {
    }

    int Module::GetNextInstName()
    {
        return _instNameCount++;
    }
}