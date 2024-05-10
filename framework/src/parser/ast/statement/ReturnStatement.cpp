// Copyright 2024 solar-mist

#include "parser/ast/statement/ReturnStatement.h"

#include <vipir/IR/Instruction/RetInst.h>

namespace parser
{
    ReturnStatement::ReturnStatement(ASTNodePtr&& returnValue)
        : mReturnValue(std::move(returnValue))
    {
    }

    vipir::Value* ReturnStatement::emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope, diagnostic::Diagnostics& diag)
    {
        vipir::Value* returnValue = nullptr;
        if (mReturnValue)
        {
            returnValue = mReturnValue->emit(builder, module, scope, diag);
        }

        return builder.CreateRet(returnValue);
    }

}