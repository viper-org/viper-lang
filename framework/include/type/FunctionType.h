// Copyright 2024 solar-mist

#ifndef VIPER_FRAMEWORK_TYPE_FUNCTION_TYPE_H
#define VIPER_FRAMEWORK_TYPE_FUNCTION_TYPE_H 1

#include "type/Type.h"

class FunctionType : public Type
{
public:
    FunctionType(Type* returnType, std::vector<Type*> argumentTypes);

    Type* getReturnType() const;
    const std::vector<Type*>& getArgumentTypes() const;

    virtual int getSize() const override;
    virtual vipir::Type* getVipirType() const override;
    virtual CastLevel castTo(Type* destType) const override;

    bool isFunctionType() const override;

    static FunctionType* Create(Type* returnType, std::vector<Type*> argumentTypes);

private:
    std::vector<Type*> mArgumentTypes;
    Type* mReturnType;
};

#endif // VIPER_FRAMEWORK_TYPE_FUNCTION_TYPE_H