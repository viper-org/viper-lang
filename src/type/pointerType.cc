#include "type/type.hh"
#include <type/pointerType.hh>

PointerType::PointerType(std::shared_ptr<Type> base)
    :Type(64), _base(base)
{
}

bool PointerType::IsPointerTy() const
{
    return true;
}

int PointerType::GetPrimitiveSize() const
{
    return 64;
}

std::shared_ptr<Type> PointerType::GetBase()
{
    return _base;
}