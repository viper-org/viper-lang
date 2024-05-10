// Copyright 2024 solar-mist

#include "parser/ast/statement/VariableDeclaration.h"

#include <vipir/IR/Instruction/AllocaInst.h>
#include <vipir/IR/Instruction/StoreInst.h>

namespace parser
{
    VariableDeclaration::VariableDeclaration(Type* type, std::string&& name, ASTNodePtr&& initialValue)
        : mName(std::move(name))
        , mInitialValue(std::move(initialValue))
    {
        mType = type;
    }

    vipir::Value* VariableDeclaration::emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope, diagnostic::Diagnostics& diag)
    {
        vipir::AllocaInst* alloca = builder.CreateAlloca(mType->getVipirType());

        if (mInitialValue)
        {
            vipir::Value* initalValue = mInitialValue->emit(builder, module, scope, diag);
            builder.CreateStore(alloca, initalValue);
        }

        scope->locals[mName] = LocalSymbol(alloca);

        return nullptr;
    }

}