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



    IfDirective::IfDirective(int start, int end, std::string condition, bool invert)
        : Directive(start, end)
        , mCondition(std::move(condition))
        , mInvert(invert)
    {
    }

    std::string_view IfDirective::getCondition() const
    {
        return mCondition;
    }

    bool IfDirective::getInvert() const
    {
        return mInvert;
    }


    EndifDirective::EndifDirective(int start, int end)
        : Directive(start, end)
    {
    }
}