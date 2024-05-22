// Copyright 2024 solar-mist

#include "parser/ast/global/Function.h"

#include "parser/ast/statement/ReturnStatement.h"

#include "symbol/NameMangling.h"

#include <vipir/IR/Function.h>
#include <vipir/IR/BasicBlock.h>
#include <vipir/IR/Constant/ConstantInt.h>

#include <algorithm>
#include <cassert>
#include <utility>

namespace parser
{
    Function::Function(std::vector<GlobalAttribute> attributes, Type* returnType, std::vector<FunctionArgument> arguments, std::string_view name, std::vector<ASTNodePtr>&& body, Scope* scope)
        : mAttributes(std::move(attributes))
        , mReturnType(returnType)
        , mArguments(std::move(arguments))
        , mName(name)
        , mBody(std::move(body))
        , mScope(scope)
    {
    }

    Type* Function::getReturnType() const
    {
        return mReturnType;
    }

    vipir::Value* Function::emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope, diagnostic::Diagnostics& diag)
    {
        if (!mBody.empty()) scope = mScope.get();

        std::vector<Type*> manglingArguments;
        std::vector<vipir::Type*> argumentTypes;
        for (auto& argument : mArguments)
        {
            manglingArguments.push_back(argument.type);
            argumentTypes.push_back(argument.type->getVipirType());
        }

        std::vector<std::string> names = scope->getNamespaces();
        names.push_back(mName);

        bool mangled = std::find_if(mAttributes.begin(), mAttributes.end(), [](const auto& attribute){
            return attribute.getType() == GlobalAttributeType::NoMangle;
        }) == mAttributes.end();

        std::string name;
        if (mangled)
            name = symbol::mangleFunctionName(names, std::move(manglingArguments));
        else
            name = mName;

        vipir::FunctionType* functionType = vipir::FunctionType::Create(mReturnType->getVipirType(), argumentTypes);
        vipir::Function* func;

        if (GlobalFunctions.contains(name))
        {
            func = GlobalFunctions[name].function;
            assert(func->getFunctionType() == functionType);
            // assert func is empty
        }
        else
        {
            func = vipir::Function::Create(functionType, module, name);
            FunctionSymbol::Create(func, name, names, mReturnType,false, mangled);
        }

        if (mBody.empty())
        {
            return func;
        }

        vipir::BasicBlock* entryBasicBlock = vipir::BasicBlock::Create("", func);
        builder.setInsertPoint(entryBasicBlock);

        int index = 0;
        for (auto& argument : mArguments)
        {
            vipir::AllocaInst* alloca = builder.CreateAlloca(argument.type->getVipirType());
            scope->locals[argument.name].alloca = alloca;

            builder.CreateStore(alloca, func->getArgument(index++));
        }

        for (auto& node : mBody)
        {
            node->emit(builder, module, scope, diag);
        }

        if (!dynamic_cast<ReturnStatement*>(mBody.back().get()))
        {
            if (mReturnType->isVoidType())
            {
                builder.CreateRet(nullptr);
            }
            else
            {
                builder.CreateRet(vipir::ConstantInt::Get(module, 0,func->getFunctionType()->getReturnType())); //TODO: get null value for type
            }
        }

        return func;
    }

}
