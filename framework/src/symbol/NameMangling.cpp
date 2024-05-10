#include <cassert>
#include "symbol/NameMangling.h"

namespace symbol
{
    std::string mangleFunctionName(std::vector<std::string_view> names, std::vector<Type*> arguments)
    {
        assert(!names.empty());
        //TODO: reserved identifiers

        if (names.size() == 1 && names.front() == "main")
        {
            return "main";
        }

        std::string res = "_F"; // function

        for (auto& name : names)
        {
            res += std::to_string(name.length());
            res += name;
        }

        if (!arguments.empty()) res += "A"; // args

        for (auto& argument : arguments)
        {
            res += argument->getMangleID();
        }

        return res;
    }
}