// Copyright 2024 solar-mist

#include "type/PointerType.h"

#include <format>
#include <algorithm>

PointerType::PointerType(Type* pointeeType)
    : Type(std::format("{}*", pointeeType->getName()))
    , mPointeeType(pointeeType)
{
}

Type* PointerType::getPointeeType() const
{
    return mPointeeType;
}

int PointerType::getSize() const
{
    return 64;
}

vipir::Type* PointerType::getVipirType() const
{
    return vipir::Type::GetPointerType(mPointeeType->getVipirType());
}

Type::CastLevel PointerType::castTo(Type* destType) const
{
    return CastLevel::Disallowed;
}

std::string PointerType::getMangleId() const
{
    return "Unimplemented function type mangling"; // TODO: implement
}

bool PointerType::isPointerType() const
{
    return true;
}

PointerType* PointerType::Get(Type* pointeeType)
{
    static std::vector<std::unique_ptr<PointerType> > pointerTypes;
    auto it = std::find_if(pointerTypes.begin(), pointerTypes.end(), [pointeeType](const auto& type){
        return type->getPointeeType() == pointeeType;
    });

    if (it != pointerTypes.end())
    {
        return it->get();
    }

    pointerTypes.push_back(std::make_unique<PointerType>(pointeeType));
    return pointerTypes.back().get();
}