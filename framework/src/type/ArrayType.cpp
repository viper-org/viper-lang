// Copyright 2024 solar-mist


#include "type/ArrayType.h"

#include <vector>
#include <vipir/Type/ArrayType.h>

#include <algorithm>
#include <format>
#include <unordered_map>

extern std::unordered_map<std::string, std::unique_ptr<Type>> types;

ArrayType::ArrayType(Type* base, int count)
    : Type(std::format("{}[{}]", base->getName(), count))
    , mBase(base)
    , mCount(count)
{
}

Type* ArrayType::getBaseType() const
{
    return mBase;
}

int ArrayType::getSize() const
{
    return mBase->getSize() * mCount;
}

vipir::Type* ArrayType::getVipirType() const
{
    return vipir::Type::GetArrayType(mBase->getVipirType(), mCount);
}

std::string ArrayType::getMangleID() const
{
    return mBase->getMangleID() + std::to_string(mCount);
}

bool ArrayType::isArrayType() const
{
    return true;
}

ArrayType* ArrayType::Create(Type* base, int count)
{
    static std::vector<std::unique_ptr<ArrayType> > arrayTypes;

    auto it = std::find_if(arrayTypes.begin(), arrayTypes.end(), [base](const std::unique_ptr<ArrayType>& type){
        return type->getBaseType() == base;
    });

    if (it != arrayTypes.end())
    {
        return it->get();
    }

    arrayTypes.push_back(std::make_unique<ArrayType>(base, count));
    return arrayTypes.back().get();
}