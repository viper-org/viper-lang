// Copyright 2024 solar-mist

#include "parser/ast/global/Function.h"
#include "parser/ast/global/StructDeclaration.h"

#include "parser/ast/statement/ReturnStatement.h"

#include <vipir/IR/Function.h>
#include <vipir/IR/BasicBlock.h>
#include <vipir/IR/Constant/ConstantInt.h>

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

    vipir::Value* Function::emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope)
    {
        if (!mBody.empty()) scope = mScope.get();

        std::vector<vipir::Type*> argumentTypes;
        for (auto& argument : mArguments)
        {
            argumentTypes.push_back(argument.type->getVipirType());
        }

        std::string name;

        if (mStruct.has_value())
            name = mangleMethodName(mStruct.value(), mName);
        else
            name = mName;

        vipir::Function* func;
        if (GlobalFunctions.contains(name))
        {
            func = GlobalFunctions[name].function;
            // assert func is empty
        }
        else
        {
            vipir::FunctionType* functionType = vipir::FunctionType::Create(mReturnType->getVipirType(), argumentTypes);
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
            node->emit(builder, module, scope);
        }

        if (!dynamic_cast<ReturnStatement*>(mBody.back().get())) {
            builder.CreateRet(vipir::ConstantInt::Get(module, 0, func->getFunctionType()->getReturnType())); //TODO: get null value for types
        }

        return func;
    }

}
