// Copyright 2024 solar-mist

#ifndef VIPER_FRAMEWORK_TYPE_INTEGER_TYPE_H
#define VIPER_FRAMEWORK_TYPE_INTEGER_TYPE_H 1

#include "type/Type.h"

class IntegerType : public Type
{
public:
    IntegerType(int bits, bool isSigned);

    virtual int getSize() const override;
    virtual vipir::Type* getVipirType() const override;
    virtual CastLevel castTo(Type* destType) const override;
    virtual std::string getImplicitCastWarning(Type* destType) const override;
    virtual std::string getMangleId() const override;

    bool isIntegerType() const override;

    bool isSigned() const;

private:
    int mBits;
    bool mSigned;
};

#endif // VIPER_FRAMEWORK_TYPE_INTEGER_TYPE_H