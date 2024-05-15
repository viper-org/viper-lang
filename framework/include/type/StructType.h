// Copyright 2024 solar-mist

#ifndef VIPER_FRAMEWORK_TYPE_STRUCT_TYPE_H
#define VIPER_FRAMEWORK_TYPE_STRUCT_TYPE_H 1

#include "type/Type.h"

#include <map>

class StructType : public Type
{
public:
    struct Field
    {
        bool priv;
        std::string name;
        Type* type;
    };

    StructType(std::vector<std::string> names, std::vector<Field> fields);

    std::string_view getName() const;
    std::vector<std::string> getNames() const;

    const std::vector<Field>& getFields() const;
    bool hasField(std::string_view fieldName);
    Field& getField(std::string_view fieldName);
    int getFieldOffset(std::string fieldName);

    int getSize() const override;
    vipir::Type* getVipirType() const override;
    std::string getMangleID() const override;

    bool isStructType() const override;

    static StructType* Get(std::vector<std::string> names);
    static StructType* Create(std::vector<std::string> names, std::vector<Field> fields);
    static void Erase(Type* type);

private:
    std::vector<std::string> mNames;
    std::vector<Field> mFields;
};

#endif // VIPER_FRAMEWORK_TYPE_STRUCT_TYPE_H
