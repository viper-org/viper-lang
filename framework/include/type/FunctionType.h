// Copyright 2024 solar-mist

#ifndef VIPER_FRAMEWORK_TYPE_FUNCTION_TYPE_H
#define VIPER_FRAMEWORK_TYPE_FUNCTION_TYPE_H 1

#include "type/Type.h"

class FunctionType : public Type
{
public:
    FunctionType(Type* returnType);

    Type* getReturnType() const;

    int getSize() const override;
    vipir::Type* getVipirType() const override;

    bool isFunctionType() const override;

    static FunctionType* Create(Type* returnType);

private:
    Type* mReturnType;
};

#endif // VIPER_FRAMEWORK_TYPE_FUNCTION_TYPE_H