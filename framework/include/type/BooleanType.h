// Copyright 2024 solar-mist

#ifndef VIPER_FRAMEWORK_TYPE_BOOLEAN_TYPE_H
#define VIPER_FRAMEWORK_TYPE_BOOLEAN_TYPE_H 1

#include "type/Type.h"

class BooleanType : public Type
{
public:
    BooleanType();

    virtual int getSize() const override;
    virtual vipir::Type* getVipirType() const override;
    virtual CastLevel castTo(Type* destType) const override;
    virtual std::string getMangleId() const override;

    bool isBooleanType() const override;
};

#endif // VIPER_FRAMEWORK_TYPE_BOOLEAN_TYPE_H