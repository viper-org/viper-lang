// Copyright 2024 solar-mist

#include "parser/ast/global/Function.h"
#include "parser/ast/global/StructDeclaration.h"
#include "parser/ast/statement/ReturnStatement.h"
#include "symbol/NameMangling.h"

#include <vipir/IR/Function.h>
#include <vipir/IR/BasicBlock.h>
#include <vipir/IR/Constant/ConstantInt.h>
#include <cassert>

namespace parser
{
    Function::Function(Type* returnType, std::vector<FunctionArgument> arguments, std::optional<std::string> struc, std::string_view name, std::vector<ASTNodePtr>&& body, Scope* scope)
        : mReturnType(returnType)
        , mArguments(std::move(arguments))
        , mStruct(struc)
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

        std::vector<std::string_view> names;
        if (mStruct.has_value())
            names.push_back(mStruct.value());
        names.push_back(mName);

        std::string name = symbol::mangleFunctionName(std::move(names), std::move(manglingArguments));

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
            GlobalFunctions[mName] = FunctionSymbol(func, false);
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
            scope->locals[argument.name] = LocalSymbol(alloca);

            builder.CreateStore(alloca, func->getArgument(index++));
        }

        for (auto& node : mBody)
        {
            node->emit(builder, module, scope, diag);
        }

        if (!dynamic_cast<ReturnStatement*>(mBody.back().get()))
        {
            builder.CreateRet(vipir::ConstantInt::Get(module, 0, func->getFunctionType()->getReturnType())); //TODO: get null value for types
        }

        return func;
    }

}
