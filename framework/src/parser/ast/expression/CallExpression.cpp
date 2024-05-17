// Copyright 2024 solar-mist


#include "parser/ast/expression/CallExpression.h"
#include "parser/ast/expression/MemberAccess.h"
#include "parser/ast/expression/VariableExpression.h"
#include "parser/ast/expression/ScopeResolution.h"

#include "parser/ast/global/StructDeclaration.h"

#include "symbol/NameMangling.h"

#include "type/PointerType.h"
#include "type/StructType.h"

#include <vipir/IR/Instruction/GEPInst.h>
#include <vipir/IR/Instruction/Instruction.h>
#include <vipir/IR/Instruction/AddrInst.h>
#include <vipir/IR/Instruction/CallInst.h>

#include <vipir/Module.h>

#include <iostream>

namespace parser
{
    CallExpression::CallExpression(ASTNodePtr function, std::vector<ASTNodePtr> parameters)
        : mFunction(std::move(function))
        , mParameters(std::move(parameters))
    {
        if (MemberAccess* member = dynamic_cast<MemberAccess*>(mFunction.get()))
        {
            std::vector<Type*> manglingArguments;

            StructType* structType = member->mPointer
                ? static_cast<StructType*>(static_cast<PointerType*>(member->mStruct->getType())->getBaseType())
                : static_cast<StructType*>(member->mStruct->getType());
            std::string methodName = member->mField;

            std::vector<std::string> structNames = structType->getNames();
            structNames.push_back(methodName);

            if (member->mStruct->getType()->isStructType())
                manglingArguments.insert(manglingArguments.begin(), PointerType::Create(member->mStruct->getType()));
            else
                manglingArguments.insert(manglingArguments.begin(), member->mStruct->getType());

            FunctionSymbol* func = FindFunction(structNames, structNames, manglingArguments);
            mType = func->returnType;
        }
        else
        {
            mType = mFunction->getType();
        }
    }

    vipir::Value* CallExpression::emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope, diagnostic::Diagnostics& diag)
    {
        std::vector<Type*> manglingArguments;
        std::vector<vipir::Value*> parameters;
        for (auto& parameter : mParameters)
        {
            manglingArguments.push_back(parameter->getType());
            parameters.push_back(parameter->emit(builder, module, scope, diag));
        }

        auto namespaceNames = scope->getNamespaces();

        if (VariableExpression* variable = dynamic_cast<VariableExpression*>(mFunction.get()))
        {
            std::string name = variable->mName;

            vipir::Function* function = FindFunction({name}, namespaceNames, manglingArguments)->function;

            return builder.CreateCall(function, std::move(parameters));
        }
        else if (MemberAccess* member = dynamic_cast<MemberAccess*>(mFunction.get()))
        {
            StructType* structType = member->mPointer
                ? static_cast<StructType*>(static_cast<PointerType*>(member->mStruct->getType())->getBaseType())
                : static_cast<StructType*>(member->mStruct->getType());
            std::string methodName = member->mField;

            std::vector<std::string> structNames = structType->getNames();
            structNames.push_back(methodName);

            vipir::Value* value = member->mStruct->emit(builder, module, scope, diag);

            if (member->mStruct->getType()->isStructType())
            {
                vipir::Value* self = vipir::getPointerOperand(value);

                vipir::Instruction* instruction = static_cast<vipir::Instruction*>(value);
                instruction->eraseFromParent();

                if (dynamic_cast<vipir::GEPInst*>(self))
                {
                    value = self;
                }
                else
                {
                    value = builder.CreateAddrOf(self);
                }

                parameters.insert(parameters.begin(), value);
                manglingArguments.insert(manglingArguments.begin(), PointerType::Create(member->mStruct->getType()));
            }
            else
            {
                mParameters.insert(mParameters.begin(), std::move(member->mStruct));
                parameters.insert(parameters.begin(), value);
                manglingArguments.insert(manglingArguments.begin(), mParameters[0]->getType());
            }

            FunctionSymbol* func = FindFunction(structNames, namespaceNames, manglingArguments);

            if (func == nullptr)
            {
                diag.compilerError(member->mFieldToken.getStart(), member->mFieldToken.getEnd(), std::format("'{}struct {}{}' has no member named '{}{}{}'",
                    fmt::bold, structType->getName(), fmt::defaults, fmt::bold, methodName, fmt::defaults));
            }
            if (func->priv && scope->findOwner() != structType)
            {
                diag.compilerError(member->mFieldToken.getStart(), member->mFieldToken.getEnd(), std::format("'{}{}{}' is a private member of '{}struct {}{}'",
                fmt::bold, member->mField, fmt::defaults, fmt::bold, structType->getName(), fmt::defaults));
            }

            vipir::Function* function = func->function;

            return builder.CreateCall(function, std::move(parameters));
        }
        else if (auto scopeRes = dynamic_cast<ScopeResolution*>(mFunction.get()))
        {
            auto names = scopeRes->getNames();

            FunctionSymbol* func = FindFunction(names, namespaceNames, manglingArguments);

            return builder.CreateCall(func->function, std::move(parameters));
        }
        else
        {
            vipir::Function* function = static_cast<vipir::Function*>(mFunction->emit(builder, module, scope, diag));

            return builder.CreateCall(function, std::move(parameters));
        }
    }
}
