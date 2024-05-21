// Copyright 2024 solar-mist


#include "type/StructType.h"
#include "type/PointerType.h"

#include "symbol/Identifier.h"

#include <vipir/Type/StructType.h>
#include <vipir/Type/PointerType.h>

#include <algorithm>
#include <map>
#include <vector>

StructType::StructType(std::vector<std::string> names, std::vector<Field> fields)
    : Type(names.back())
    , mNames(std::move(names))
    , mFields(std::move(fields))
{
    symbol::AddIdentifier(getMangleID(), mNames);
}

std::string_view StructType::getName() const
{
    return mNames.back();
}

std::vector<std::string> StructType::getNames() const
{
    return mNames;
}

std::vector<StructType::Field>& StructType::getFields()
{
    return mFields;
}

bool StructType::hasField(std::string_view fieldName)
{
    return std::find_if(mFields.begin(), mFields.end(), [&fieldName](const Field& field){
        return fieldName == field.name;
    }) != mFields.end();
}

StructType::Field* StructType::getField(std::string_view fieldName)
{
    auto it = std::find_if(mFields.begin(), mFields.end(), [&fieldName](const Field& field){
        return fieldName == field.name;
    });
    if (it == mFields.end()) return nullptr;

    return &*it;
}

int StructType::getFieldOffset(std::string fieldName)
{
    return std::find_if(mFields.begin(), mFields.end(), [&fieldName](const Field& field){
        return fieldName == field.name;
    }) - mFields.begin();
}

int StructType::getSize() const
{
    int size = 0;
    for (auto& field : mFields)
        size += field.type->getSize();
    
    return size;
}

vipir::Type* StructType::getVipirType() const
{
    std::vector<vipir::Type*> fieldTypes;
    for (auto [_, _x, field] : mFields)
    {
        if (field->isPointerType())
        {
            // struct types with a pointer to themselves cannot be emitted normally
            if (static_cast<PointerType*>(field)->getBaseType() == this)
            {
                fieldTypes.push_back(vipir::PointerType::GetPointerType(vipir::Type::GetIntegerType(8)));
                continue;
            }
        }
        fieldTypes.push_back(field->getVipirType());
    }
    return vipir::Type::GetStructType(std::move(fieldTypes));
}

std::string StructType::getMangleID() const
{
    std::string ret = "S";
    for (auto name : mNames)
    {
        ret += std::to_string(name.length()) + name;
    }
    return ret;
}

bool StructType::isStructType() const
{
    return true;
}


static std::vector<std::unique_ptr<StructType> > structTypes;

StructType* StructType::Get(std::string name)
{
    auto it = std::find_if(structTypes.begin(), structTypes.end(), [&name](const auto& type){
        return type->getMangleID() == name;
    });
    if (it == structTypes.end()) return nullptr;
    return it->get();
}

StructType* StructType::Create(std::vector<std::string> names, std::vector<StructType::Field> fields)
{
    auto it = std::find_if(structTypes.begin(), structTypes.end(), [&names](const auto& type){
        return type->mNames == names;
    });

    if (it != structTypes.end())
    {
        return it->get();
    }

    structTypes.push_back(std::make_unique<StructType>(names, std::move(fields)));
    return structTypes.back().get();
}

void StructType::Erase(Type* type)
{
    auto structType = static_cast<StructType*>(type);
    
    structTypes.erase(std::remove_if(structTypes.begin(), structTypes.end(), [structType](const auto& type){
        return type.get() == structType;
    }), structTypes.end());
}