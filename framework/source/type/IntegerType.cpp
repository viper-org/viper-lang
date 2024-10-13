// Copyright 2024 solar-mist

#include "type/IntegerType.h"

#include "diagnostic/Diagnostic.h"

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

Type::CastLevel IntegerType::castTo(Type* destType) const
{
    if (destType->isIntegerType())
    {
        if (destType->getSize() < mBits)
        {
            return Type::CastLevel::ImplicitWarning;
        }
        return Type::CastLevel::Implicit;
    }
    return Type::CastLevel::Disallowed;
}

std::string IntegerType::getImplicitCastWarning(Type* destType) const
{
    return std::format("potential loss of data casting '{}{}{}' to '{}{}{}'",
        fmt::bold, mName, fmt::defaults,
        fmt::bold, destType->getName(), fmt::defaults);
}

bool IntegerType::isIntegerType() const
{
    return true;
}

bool IntegerType::isSigned() const
{
    return mSigned;
}