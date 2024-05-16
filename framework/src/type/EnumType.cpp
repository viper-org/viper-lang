// Copyright 2024 solar-mist


#include "type/EnumType.h"

#include <unordered_map>

EnumType::EnumType(std::vector<std::string> names)
    : Type(names.back())
    , mNames(std::move(names))
{
}

int EnumType::getSize() const
{
    return 32;
}

vipir::Type* EnumType::getVipirType() const
{
    return vipir::Type::GetIntegerType(32);
}

std::string EnumType::getMangleID() const
{
    std::string ret = "_E";
    for (auto& name : mNames)
    {
        ret += std::to_string(name.length());
        ret += name;
    }

    return ret;
}

bool EnumType::isEnumType() const
{
    return true;
}

extern std::unordered_map<std::string, std::unique_ptr<Type>> types;
EnumType* EnumType::Create(std::vector<std::string> names)
{
    std::unique_ptr<Type> type = std::make_unique<EnumType>(names);
    std::string mangleID = type->getMangleID();
    types[mangleID] = std::move(type);

    return static_cast<EnumType*>(types[mangleID].get());
}