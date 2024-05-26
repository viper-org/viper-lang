// Copyright 2024 solar-mist

#ifndef VIPER_FRAMEWORK_TYPE_FUNCTION_TYPE_H
#define VIPER_FRAMEWORK_TYPE_FUNCTION_TYPE_H 1

#include "type/Type.h"

class FunctionType : public Type
{
public:
    FunctionType(Type* returnType, std::vector<Type*> arguments);

    Type* getReturnType() const;
    const std::vector<Type*>& getArgumentTypes() const;

    int getSize() const override;
    vipir::Type* getVipirType() const override;
    std::string getMangleID() const override;

    bool isFunctionType() const override;

    static FunctionType* Create(Type* returnType, std::vector<Type*> arguments);

private:
    Type* mReturnType;
    std::vector<Type*> mArguments;
};

#endif // VIPER_FRAMEWORK_TYPE_FUNCTION_TYPE_H