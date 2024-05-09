// Copyright 2024 solar-mist


#include "parser/ast/expression/CallExpression.h"
#include "parser/ast/expression/MemberAccess.h"
#include "parser/ast/global/StructDeclaration.h"
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

    vipir::Value* CallExpression::emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope)
    {

        std::vector<vipir::Value*> parameters;
        for (auto& parameter : mParameters)
        {
            parameters.push_back(parameter->emit(builder, module, scope));
        }

        if (MemberAccess* member = dynamic_cast<MemberAccess*>(mFunction.get()))
        {
            StructType* structType = member->mPointer
                ? static_cast<StructType*>(static_cast<PointerType*>(member->mStruct->getType())->getBaseType())
                : static_cast<StructType*>(member->mStruct->getType());
            std::string_view methodName = member->mField;
            std::string mangledName = mangleMethodName(structType->getName(), methodName);

            if (GlobalFunctions.at(mangledName).priv)
            {
                if (scope->owner != structType)
                { // TODO: Proper error
                    std::cerr << std::format("{} is a private member of struct {}\n", member->mField, structType->getName());
                }
            }
            vipir::Value* value = member->mStruct->emit(builder, module, scope);

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

            }
            else
            {
                mParameters.insert(mParameters.begin(), std::move(member->mStruct));
                parameters.insert(parameters.begin(), value);
            }

            vipir::Function* function = GlobalFunctions.at(mangledName).function;

            return builder.CreateCall(function, std::move(parameters));
        }
        else
        {
            vipir::Function* function = static_cast<vipir::Function*>(mFunction->emit(builder, module, scope));

            return builder.CreateCall(function, std::move(parameters));
        }
    }
}
