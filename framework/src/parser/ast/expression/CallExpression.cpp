// Copyright 2024 solar-mist


#include "parser/ast/expression/CallExpression.h"
#include "parser/ast/expression/MemberAccess.h"
#include "parser/ast/global/StructDeclaration.h"
#include "parser/ast/expression/VariableExpression.h"
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
        mType = mFunction->getType();
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

        if (VariableExpression* variable = dynamic_cast<VariableExpression*>(mFunction.get()))
        {
            std::string name = variable->mName;
            std::string mangledName = symbol::mangleFunctionName({name}, std::move(manglingArguments));

            vipir::Function* function = GlobalFunctions.at(mangledName).function;

            return builder.CreateCall(function, std::move(parameters));
        }
        else if (MemberAccess* member = dynamic_cast<MemberAccess*>(mFunction.get()))
        {
            StructType* structType = member->mPointer
                ? static_cast<StructType*>(static_cast<PointerType*>(member->mStruct->getType())->getBaseType())
                : static_cast<StructType*>(member->mStruct->getType());
            std::string_view methodName = member->mField;
            std::string mangledName;

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

                mParameters.insert(mParameters.begin(), std::move(member->mStruct));
                parameters.insert(parameters.begin(), value);
                manglingArguments.insert(manglingArguments.begin(), mParameters[0]->getType());

                mangledName = symbol::mangleFunctionName({structType->getName(), methodName}, std::move(manglingArguments));
            }
            else
            {
                mParameters.insert(mParameters.begin(), std::move(member->mStruct));
                parameters.insert(parameters.begin(), value);
                manglingArguments.insert(manglingArguments.begin(), mParameters[0]->getType());
                mangledName = symbol::mangleFunctionName({structType->getName(), methodName}, std::move(manglingArguments));
            }

            if (GlobalFunctions.at(mangledName).priv)
            {
                if (scope->owner != structType)
                { // TODO: Proper error
                    std::cerr << std::format("{} is a private member of struct {}\n", member->mField, structType->getName());
                }
            }

            if (GlobalFunctions.find(mangledName) == GlobalFunctions.end())
            {
                diag.compilerError(member->mFieldToken.getStart(), member->mFieldToken.getEnd(), std::format("'{}struct {}{}' has no member named '{}{}{}'",
                    fmt::bold, structType->getName(), fmt::defaults, fmt::bold, methodName, fmt::defaults));
            }
            if (GlobalFunctions.at(mangledName).priv && scope->owner != structType)
            {
                diag.compilerError(member->mFieldToken.getStart(), member->mFieldToken.getEnd(), std::format("'{}{}{}' is a private member of '{}struct {}{}'",
                fmt::bold, member->mField, fmt::defaults, fmt::bold, structType->getName(), fmt::defaults));
            }

            if (GlobalFunctions.find(mangledName) == GlobalFunctions.end())
            {
                diag.compilerError(member->mFieldToken.getStart(), member->mFieldToken.getEnd(), std::format("'{}struct {}{}' has no member named '{}{}{}'",
                    fmt::bold, structType->getName(), fmt::defaults, fmt::bold, methodName, fmt::defaults));
            }
            if (GlobalFunctions.at(mangledName).priv && scope->owner != structType)
            {
                diag.compilerError(member->mFieldToken.getStart(), member->mFieldToken.getEnd(), std::format("'{}{}{}' is a private member of '{}struct {}{}'",
                fmt::bold, member->mField, fmt::defaults, fmt::bold, structType->getName(), fmt::defaults));
            }

            vipir::Function* function = GlobalFunctions.at(mangledName).function;

            return builder.CreateCall(function, std::move(parameters));
        }
        else
        {
            vipir::Function* function = static_cast<vipir::Function*>(mFunction->emit(builder, module, scope, diag));

            return builder.CreateCall(function, std::move(parameters));
        }
    }
}
