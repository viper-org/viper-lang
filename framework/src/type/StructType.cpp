// Copyright 2024 solar-mist


#include "type/StructType.h"

#include <vector>
#include <vipir/Type/StructType.h>

#include <algorithm>
#include <map>

StructType::StructType(std::string name, std::vector<Field> fields)
    : mName(std::move(name))
    , mFields(std::move(fields))
{
}

std::string_view StructType::getName() const
{
    return mName;
}

const std::vector<StructType::Field>& StructType::getFields() const
{
    return mFields;
}

StructType::Field& StructType::getField(std::string fieldName)
{
    return *std::find_if(mFields.begin(), mFields.end(), [&fieldName](const Field& field){
        return fieldName == field.name;
    });
}

int StructType::getFieldOffset(std::string fieldName)
{
    return std::find_if(mFields.begin(), mFields.end(), [&fieldName](const Field& field){
        return fieldName == field.name;
    }) - mFields.begin();
}

int StructType::getSize() const
{
    return 64;
}

vipir::Type* StructType::getVipirType() const
{
    std::vector<vipir::Type*> fieldTypes;
    for (auto [_, _x, field] : mFields)
    {
        fieldTypes.push_back(field->getVipirType());
    }
    return vipir::Type::GetStructType(std::move(fieldTypes));
}

std::string StructType::getMangleID() const
{
    return "S" + std::to_string(mName.length()) + mName;
}

bool StructType::isStructType() const
{
    return true;
}


static std::map<std::string, std::unique_ptr<StructType> > structTypes;

StructType* StructType::Get(std::string name)
{
    return structTypes.at(name).get();
}

StructType* StructType::Create(std::string name, std::vector<StructType::Field> fields)
{
    auto it = std::find_if(structTypes.begin(), structTypes.end(), [&name](const auto& type){
        return type.first == name;
    });

    if (it != structTypes.end())
    {
        return it->second.get();
    }

    structTypes[name] = std::make_unique<StructType>(name, std::move(fields));
    return structTypes.at(name).get();
}
