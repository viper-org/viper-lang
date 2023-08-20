// Copyright 2023 solar-mist


#include "parser/ast/global/Function.h"

#include "environment/Environment.h"

#include <vipir/IR/Function.h>
#include <vipir/IR/BasicBlock.h>

namespace parsing
{
    Function::Function(Type* type, const std::string& name, std::vector<FunctionArgument> arguments, std::vector<ASTNodePtr> body)
        : mReturnType(type)
        , mName(name)
        , mArguments(std::move(arguments))
        , mBody(std::move(body))
    {
    }

    Type* Function::getReturnType() const
    {
        return mReturnType;
    }

    std::string_view Function::getName() const
    {
        return mName;
    }

    const std::vector<FunctionArgument>& Function::getArguments() const
    {
        return mArguments;
    }

    const std::vector<ASTNodePtr>& Function::getBody() const
    {
        return mBody;
    }

    vipir::Value* Function::emit(vipir::Builder& builder, vipir::Module& module)
    {
        std::vector<vipir::Type*> argumentTypes;
        for (auto argument : mArguments)
        {
            argumentTypes.push_back(argument.getType()->getVipirType());
        }
        vipir::FunctionType* functionType = vipir::FunctionType::Get(mReturnType->getVipirType(), argumentTypes);
        vipir::Function* function = vipir::Function::Create(functionType, module, mName);
        functions[mName] = function;

        vipir::BasicBlock* entryBB = vipir::BasicBlock::Create("", function);
        builder.setInsertPoint(entryBB);

        int index = 0;
        for (auto argument : mArguments)
        {
            vipir::AllocaInst* alloca = builder.CreateAlloca(argument.getType()->getVipirType());
            variables[argument.getName()] = alloca;
            builder.CreateStore(alloca, function->getArgument(index++));
        }

        for (auto& node : mBody)
        {
            node->emit(builder, module);
        }

        return function;
    }
}