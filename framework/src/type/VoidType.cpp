// Copyright 2024 solar-mist


#include "type/VoidType.h"

VoidType::VoidType()
{
}

int VoidType::getSize() const
{
    return 0;
}

vipir::Type* VoidType::getVipirType() const
{
    return vipir::Type::GetVoidType();
}

bool VoidType::isVoidType() const
{
    return true;
}