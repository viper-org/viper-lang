// Copyright 2024 solar-mist


#include "preprocessor/Directive.h"

namespace preprocessor
{
    IncludeDirective::IncludeDirective(int start, int end, std::string path)
        : Directive(start, end)
        , mPath(path)
    {
    }

    std::string_view IncludeDirective::getPath() const
    {
        return mPath;
    }
}