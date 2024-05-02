// Copyright 2024 solar-mist


#include "preprocessor/Directive.h"

namespace preprocessor
{
    IncludeDirective::IncludeDirective(int start, int end, std::string path)
        : Directive(start, end)
        , mPath(std::move(path))
    {
    }

    std::string_view IncludeDirective::getPath() const
    {
        return mPath;
    }


    DefineDirective::DefineDirective(int start, int end, std::string name, std::string value)
        : Directive(start, end)
        , mName(std::move(name))
        , mValue(std::move(value))
    {
    }

    std::string_view DefineDirective::getName() const
    {
        return mName;
    }

    std::string_view DefineDirective::getValue() const
    {
        return mValue;
    }
}