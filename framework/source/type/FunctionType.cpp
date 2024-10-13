// Copyright 2024 solar-mist

#include "type/FunctionType.h"

#include <vipir/Type/FunctionType.h>

#include <algorithm>
#include <format>

FunctionType::FunctionType(Type* returnType)
    : Type(std::format("{}()", returnType->getName()))
    , mReturnType(returnType)
{
}

Type* FunctionType::getReturnType() const
{
    return mReturnType;
}

int FunctionType::getSize() const
{
    return 0;
}

vipir::Type* FunctionType::getVipirType() const
{
    return vipir::FunctionType::Create(mReturnType->getVipirType(), {});
}

Type::CastLevel FunctionType::castTo(Type* destType) const
{
    return CastLevel::Disallowed;
}

bool FunctionType::isFunctionType() const
{
    return true;
}

FunctionType* FunctionType::Create(Type* returnType)
{
    static std::vector<std::unique_ptr<FunctionType> > functionTypes;
    auto it = std::find_if(functionTypes.begin(), functionTypes.end(), [returnType](const auto& type){
        return type->getReturnType() == returnType;
    });

    if (it != functionTypes.end())
    {
        return it->get();
    }

    functionTypes.push_back(std::make_unique<FunctionType>(returnType));
    return functionTypes.back().get();
}