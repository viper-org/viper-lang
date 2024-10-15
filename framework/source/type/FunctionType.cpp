// Copyright 2024 solar-mist

#include "type/FunctionType.h"

#include <vipir/Type/FunctionType.h>

#include <algorithm>
#include <format>

FunctionType::FunctionType(Type* returnType, std::vector<Type*> argumentTypes)
    : Type(std::format("{}(", returnType->getName()))
    , mArgumentTypes(std::move(argumentTypes))
    , mReturnType(returnType)
{
    if (!mArgumentTypes.empty())
    {
        for (int i = 0; i < mArgumentTypes.size()-1; ++i)
        {
            mName += std::format("{}, ", mArgumentTypes[i]->getName());
        }
        mName += std::format("{})", mArgumentTypes.back()->getName());
    }
    else
    {
        mName += ")";
    }
}

Type* FunctionType::getReturnType() const
{
    return mReturnType;
}

const std::vector<Type*>& FunctionType::getArgumentTypes() const
{
    return mArgumentTypes;
}

int FunctionType::getSize() const
{
    return 0;
}

vipir::Type* FunctionType::getVipirType() const
{
    std::vector<vipir::Type*> argumentTypes;
    for (auto& argumentType : mArgumentTypes)
    {
        argumentTypes.push_back(argumentType->getVipirType());
    }
    return vipir::FunctionType::Create(mReturnType->getVipirType(), std::move(argumentTypes));
}

Type::CastLevel FunctionType::castTo(Type* destType) const
{
    return CastLevel::Disallowed;
}

std::string FunctionType::getMangleId() const
{
    return "Unimplemented function type mangling"; // TODO: implement
}

bool FunctionType::isFunctionType() const
{
    return true;
}

FunctionType* FunctionType::Create(Type* returnType, std::vector<Type*> argumentTypes)
{
    static std::vector<std::unique_ptr<FunctionType> > functionTypes;
    auto it = std::find_if(functionTypes.begin(), functionTypes.end(), [returnType, &argumentTypes](const auto& type){
        return type->getReturnType() == returnType && type->getArgumentTypes() == argumentTypes;
    });

    if (it != functionTypes.end())
    {
        return it->get();
    }

    functionTypes.push_back(std::make_unique<FunctionType>(returnType, std::move(argumentTypes)));
    return functionTypes.back().get();
}