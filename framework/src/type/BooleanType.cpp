// Copyright 2024 solar-mist


#include "type/BooleanType.h"

BooleanType::BooleanType()
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

bool BooleanType::isBooleanType() const
{
    return true;
}