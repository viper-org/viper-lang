// Copyright 2024 solar-mist

#include "type/IntegerType.h"

#include <format>

IntegerType::IntegerType(int bits, bool isSigned)
    : Type(std::format("{}{}", (isSigned ? "i" : "u"), bits))
    , mBits(bits)
    , mSigned(isSigned)
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

bool IntegerType::isSigned() const
{
    return mSigned;
}