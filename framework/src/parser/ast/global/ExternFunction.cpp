// Copyright 2023 solar-mist


#include "parser/ast/global/ExternFunction.h"

#include "environment/Environment.h"

#include <vipir/IR/Function.h>

namespace parsing
{
    ExternFunction::ExternFunction(Type* type, const std::string& name)
        : mReturnType(type)
        , mName(name)
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

    vipir::Value* ExternFunction::emit(vipir::Builder& builder, vipir::Module& module)
    {
        vipir::FunctionType* functionType = vipir::FunctionType::Get(mReturnType->getVipirType(), {});
        vipir::Function* function = vipir::Function::Create(functionType, module, mName);

        functions[mName] = function;

        return function;
    }
}