// Copyright 2024 solar-mist


#include "type/PointerType.h"

#include <vector>
#include <vipir/Type/PointerType.h>

#include <algorithm>
#include <unordered_map>

extern std::unordered_map<std::string, std::unique_ptr<Type>> types;

PointerType::PointerType(Type* base)
    : mBase(base)
{
}

Type* PointerType::getBaseType() const
{
    return mBase;
}

int PointerType::getSize() const
{
    return 64;
}

vipir::Type* PointerType::getVipirType() const
{
    return vipir::Type::GetPointerType(mBase->getVipirType());
}

bool PointerType::isPointerType() const
{
    return true;
}

PointerType* PointerType::Create(Type* base)
{
    static std::vector<std::unique_ptr<PointerType> > pointerTypes;

    auto it = std::find_if(pointerTypes.begin(), pointerTypes.end(), [base](const std::unique_ptr<PointerType>& type){
        return type->getBaseType() == base;
    });

    if (it != pointerTypes.end())
    {
        return it->get();
    }

    pointerTypes.push_back(std::make_unique<PointerType>(base));
    return pointerTypes.back().get();
}