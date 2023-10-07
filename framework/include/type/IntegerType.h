// Copyright 2023 solar-mist

#ifndef VIPER_FRAMEWORK_TYPE_INTEGER_TYPE_H
#define VIPER_FRAMEWORK_TYPE_INTEGER_TYPE_H 1

#include "type/Type.h"

class IntegerType : public Type
{
public:
    IntegerType(int bits);

    int getSize() const override;
    vipir::Type* getVipirType() const override;

    bool isIntegerType() const override;

private:
    int mBits;
};

#endif