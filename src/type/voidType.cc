#include <type/voidType.hh>

VoidType::VoidType()
    :Type(0)
{
}

bool VoidType::IsVoidTy() const
{
    return true;
}