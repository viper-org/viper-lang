// Copyright 2024 solar-mist

#ifndef VIPER_FRAMEWORK_TYPE_POINTER_TYPE_H
#define VIPER_FRAMEWORK_TYPE_POINTER_TYPE_H 1

#include "type/Type.h"

class PointerType : public Type
{
public:
    PointerType(Type* base);

    Type* getBaseType() const;

    int getSize() const override;
    vipir::Type* getVipirType() const override;
    std::string getMangleID() const override;

    bool isPointerType() const override;

    static PointerType* Create(Type* base);

private:
    Type* mBase;
};

#endif // VIPER_FRAMEWORK_TYPE_POINTER_TYPE_H