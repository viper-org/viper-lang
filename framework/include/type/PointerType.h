// Copyright 2023 solar-mist

#ifndef VIPER_FRAMEWORK_TYPE_POINTER_TYPE_H
#define VIPER_FRAMEWORK_TYPE_POINTER_TYPE_H 1

#include "type/Type.h"

class PointerType : public Type
{
public:
    PointerType(Type* baseType);

    int getSize() const override;
    vipir::Type* getVipirType() const override;

private:
    Type* mBaseType;
};

#endif