// Copyright 2024 solar-mist

#include "parser/ast/statement/VariableDeclaration.h"

#include <vipir/IR/Instruction/AllocaInst.h>
#include <vipir/IR/Instruction/StoreInst.h>

namespace parser
{
    VariableDeclaration::VariableDeclaration(std::string&& name, ASTNodePtr&& initialValue)
        : mName(std::move(name))
        , mInitialValue(std::move(initialValue))
    {
    }

    vipir::Value* VariableDeclaration::emit(vipir::IRBuilder& builder, vipir::Module& module)
    {
        vipir::AllocaInst* alloca = builder.CreateAlloca(mName);

        if (mInitialValue)
        {
            vipir::Value* initalValue = mInitialValue->emit(builder, module);
            builder.CreateStore(alloca, initalValue);
        }

        return nullptr;
    }

}