// Copyright 2023 solar-mist


#include "parser/ast/global/FunctionArgument.h"

namespace parsing
{
    FunctionArgument::FunctionArgument(const std::string& name, Type* type)
        : mName(name)
        , mType(type)
    {
    }

    const std::string& FunctionArgument::getName() const
    {
        return mName;
    }

    Type* FunctionArgument::getType() const
    {
        return mType;
    }
}