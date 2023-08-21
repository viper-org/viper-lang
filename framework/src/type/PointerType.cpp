// Copyright 2023 solar-mist


#include "type/PointerType.h"

#include "vipir/Type/PointerType.h"

PointerType::PointerType(Type* baseType)
    : mBaseType(baseType)
{
}

int PointerType::getSize() const
{
    return 64;
}

vipir::Type* PointerType::getVipirType() const
{
    return vipir::PointerType::Get(mBaseType->getVipirType());
}