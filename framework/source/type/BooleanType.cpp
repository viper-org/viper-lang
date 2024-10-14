// Copyright 2024 solar-mist

#include "type/BooleanType.h"

#include <format>

BooleanType::BooleanType()
    : Type("bool")
{
}

int BooleanType::getSize() const
{
    return 8;
}

Type::CastLevel BooleanType::castTo(Type* destType) const
{
    if (destType->isIntegerType())
    {
        return Type::CastLevel::Implicit;
    }
    return Type::CastLevel::Disallowed;
}

vipir::Type* BooleanType::getVipirType() const
{
    return vipir::Type::GetBooleanType();
}

bool BooleanType::isBooleanType() const
{
    return true;
}