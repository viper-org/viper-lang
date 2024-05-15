// Copyright 2024 solar-mist


#include "parser/ast/global/GlobalAttribute.h"

namespace parser
{
    GlobalAttribute::GlobalAttribute(GlobalAttributeType type)
        : mType(type)
    {
    }

    GlobalAttributeType GlobalAttribute::getType() const
    {
        return mType;
    }
}