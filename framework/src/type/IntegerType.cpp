// Copyright 2023 solar-mist


#include "type/IntegerType.h"

IntegerType::IntegerType(int bits)
    : mBits(bits)
{
}

int IntegerType::getSize() const
{
    return mBits;
}

vipir::Type* IntegerType::getVipirType() const
{
    return vipir::Type::GetIntegerType(mBits);
}

bool IntegerType::isIntegerType() const
{
    return true;
}