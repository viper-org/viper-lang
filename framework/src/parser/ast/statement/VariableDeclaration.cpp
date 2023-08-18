// Copyright 2023 solar-mist


#include "parser/ast/statement/VariableDeclaration.h"

#include "environment/Environment.h"

#include <vipir/IR/Instruction/AllocaInst.h>

namespace parsing
{
    VariableDeclaration::VariableDeclaration(Type* type, std::string&& name, ASTNodePtr value)
        : mName(name)
        , mValue(std::move(value))
    {
        mType = type;
    }

    vipir::Value* VariableDeclaration::emit(vipir::Builder& builder, vipir::Module& module)
    {
        vipir::AllocaInst* alloca = builder.CreateAlloca(mType->getVipirType());

        if (mValue)
        {
            vipir::Value* initValue = mValue->emit(builder, module);
            builder.CreateStore(alloca, initValue);
        }

        variables[mName] = alloca;

        return alloca;
    }
}