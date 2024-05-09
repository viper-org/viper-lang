// Copyright 2024 solar-mist


#include <iostream>
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

std::string IntegerType::getMangleID() const
{
    switch (mBits)
    {
        case 8:
            return "b"; // byte
        case 16:
            return "s"; // short
        case 32:
            return "i"; // int
        case 64:
            return "l"; // long

        default:
            std::cerr << "Warning: Unknown integer type size for mangling: " << mBits << ". Defaulting to 'I" << mBits << "'.\n";
            return "I" + std::to_string(mBits);
    }
}

bool IntegerType::isIntegerType() const
{
    return true;
}