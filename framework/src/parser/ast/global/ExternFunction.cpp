// Copyright 2023 solar-mist


#include "parser/ast/global/ExternFunction.h"

#include "environment/Environment.h"

#include <vipir/IR/Function.h>

namespace parsing
{
    ExternFunction::ExternFunction(Type* type, const std::string& name, std::vector<FunctionArgument> arguments)
        : mReturnType(type)
        , mName(name)
        , mArguments(std::move(arguments))
    {
    }

    Type* ExternFunction::getReturnType() const
    {
        return mReturnType;
    }

    std::string_view ExternFunction::getName() const
    {
        return mName;
    }

    const std::vector<FunctionArgument>& ExternFunction::getArguments() const
    {
        return mArguments;
    }

    vipir::Value* ExternFunction::emit(vipir::Builder& builder, vipir::Module& module)
    {
        std::vector<vipir::Type*> argumentTypes;
        for (auto argument : mArguments)
        {
            argumentTypes.push_back(argument.getType()->getVipirType());
        }
        vipir::FunctionType* functionType = vipir::FunctionType::Get(mReturnType->getVipirType(), argumentTypes);
        vipir::Function* function = vipir::Function::Create(functionType, module, mName);

        functions[mName] = function;

        return function;
    }
}