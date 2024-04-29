// Copyright 2024 solar-mist


#include "type/StructType.h"

#include <vector>
#include <vipir/Type/StructType.h>

#include <algorithm>
#include <map>

StructType::StructType(std::vector<Field> fields)
    : mFields(std::move(fields))
{
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
    for (auto [_, field] : mFields)
    {
        fieldTypes.push_back(field->getVipirType());
    }
    return vipir::Type::GetStructType(std::move(fieldTypes));
}

bool StructType::isPointerType() const
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

    structTypes[name] = std::make_unique<StructType>(std::move(fields));
    return structTypes.at(name).get();
}