// Copyright 2023 solar-mist

#ifndef VIPER_FRAMEWORK_TYPE_VOID_TYPE_H
#define VIPER_FRAMEWORK_TYPE_VOID_TYPE_H 1

#include "type/Type.h"

class VoidType : public Type
{
public:
    VoidType();

    int getSize() const override;
    vipir::Type* getVipirType() const override;
};

#endif