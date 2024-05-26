// Copyright 2024 solar-mist


#include "type/FunctionType.h"

#include <vipir/Type/FunctionType.h>

#include <algorithm>
#include <format>

FunctionType::FunctionType(Type* returnType, std::vector<Type*> arguments)
    : Type(std::format("{}(", returnType->getName()))
    , mReturnType(returnType)
    , mArguments(std::move(arguments))
{
    if (!mArguments.empty())
    {
        for (int i = 0; i < mArguments.size()-1; ++i)
        {
            mName += std::format("{}, ", mArguments[i]->getName());
        }
        mName += std::format("{})", mArguments.back()->getName());
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
    return mArguments;
}

int FunctionType::getSize() const
{
    return 0;
}

vipir::Type* FunctionType::getVipirType() const
{
    std::vector<vipir::Type*> arguments;
    for (auto argument : mArguments)
    {
        arguments.push_back(argument->getVipirType());
    }
    return vipir::FunctionType::Create(mReturnType->getVipirType(), std::move(arguments));
}

std::string FunctionType::getMangleID() const
{
    std::string ret = "F" + mReturnType->getMangleID();
    for (auto argument : mArguments)
    {
        ret += argument->getMangleID();
    }
    return ret;
}

bool FunctionType::isFunctionType() const
{
    return true;
}

FunctionType* FunctionType::Create(Type* returnType, std::vector<Type*> arguments)
{
    static std::vector<std::unique_ptr<FunctionType> > functionTypes;
    auto it = std::find_if(functionTypes.begin(), functionTypes.end(), [returnType, &arguments](const auto& type){
        return type->getReturnType() == returnType && type->getArgumentTypes() == arguments;
    });

    if (it != functionTypes.end())
    {
        return it->get();
    }

    functionTypes.push_back(std::make_unique<FunctionType>(returnType, std::move(arguments)));
    return functionTypes.back().get();
}