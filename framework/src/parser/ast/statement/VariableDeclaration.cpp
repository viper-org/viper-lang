// Copyright 2023 solar-mist


#include "parser/ast/statement/VariableDeclaration.h"

#include <vipir/IR/Instruction/AllocaInst.h>

namespace parsing
{
    VariableDeclaration::VariableDeclaration(std::string&& name, ASTNodePtr value)
        : mName(name)
        , mValue(std::move(value))
    {
    }

    vipir::Value* VariableDeclaration::emit(vipir::Builder& builder, vipir::Module& module)
    {
        vipir::AllocaInst* alloca = builder.CreateAlloca(vipir::Type::GetIntegerType(32));

        if (mValue)
        {
            vipir::Value* initValue = mValue->emit(builder, module);
            builder.CreateStore(alloca, initValue);
        }

        return alloca;
    }
}