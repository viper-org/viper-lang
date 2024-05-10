// Copyright 2024 solar-mist


#include "type/BooleanType.h"

BooleanType::BooleanType()
    : Type("bool")
{
}

int BooleanType::getSize() const
{
    return 8;
}

vipir::Type* BooleanType::getVipirType() const
{
    return vipir::Type::GetBooleanType();
}

std::string BooleanType::getMangleID() const
{
    return "B";
}

bool BooleanType::isBooleanType() const
{
    return true;
}