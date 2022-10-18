#include <type/integerType.hh>

IntegerType::IntegerType(int bits)
    :Type(bits)
{
}

bool IntegerType::IsIntegerTy() const
{
    return true;
}