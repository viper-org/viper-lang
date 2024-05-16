// Copyright 2024 solar-mist

#ifndef VIPER_FRAMEWORK_TYPE_ENUM_TYPE_H
#define VIPER_FRAMEWORK_TYPE_ENUM_TYPE_H 1

#include "type/Type.h"

class EnumType : public Type
{
public:
    EnumType(std::vector<std::string> names);

    int getSize() const override;
    vipir::Type* getVipirType() const override;
    std::string getMangleID() const override;
    bool isEnumType() const override;

    static EnumType* Create(std::vector<std::string> names);

private:
    std::vector<std::string> mNames;
};

#endif // VIPER_FRAMEWORK_TYPE_ENUM_TYPE_H