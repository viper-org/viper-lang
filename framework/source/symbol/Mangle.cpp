// Copyright 2024 solar-mist

#include "symbol/Mangle.h"

namespace mangle
{
    std::string MangleFunction(std::string_view name, FunctionType* type)
    {
        if (name == "main") return "main";
        std::string mangled = "_F";
        mangled += std::to_string(name.length());
        mangled += name;
        for (auto& argumentType : type->getArgumentTypes())
        {
            mangled += argumentType->getMangleId();
        }

        return mangled;
    }
}