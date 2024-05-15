// Copyright 2024 solar-mist


#include <iostream>
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

std::string IntegerType::getMangleID() const
{
    std::string ret;
    if (!mSigned) ret = "u";
    switch (mBits)
    {
        case 8:
            ret += "b"; // byte
            break;
        case 16:
            ret += "s"; // short
            break;
        case 32:
            ret += "i"; // int
            break;
        case 64:
            ret += "l"; // long
            break;

        default:
            std::cerr << "Warning: Unknown integer type size for mangling: " << mBits << ". Defaulting to 'I" << mBits << "'.\n";
            return (mSigned ? "I" : "U") + std::to_string(mBits);
    }
    return ret;
}

bool IntegerType::isIntegerType() const
{
    return true;
}

bool IntegerType::isSigned() const
{
    return mSigned;
}