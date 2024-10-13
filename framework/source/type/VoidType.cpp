// Copyright 2024 solar-mist

#include "type/VoidType.h"

#include <format>

VoidType::VoidType()
    : Type("void")
{
}

int VoidType::getSize() const
{
    return 0;
}

Type::CastLevel VoidType::castTo(Type* destType) const
{
    return CastLevel::Disallowed;
}

vipir::Type* VoidType::getVipirType() const
{
    return vipir::Type::GetVoidType();
}

bool VoidType::isVoidType() const
{
    return true;
}