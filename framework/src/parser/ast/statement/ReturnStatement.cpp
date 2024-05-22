// Copyright 2024 solar-mist

#include "parser/ast/statement/ReturnStatement.h"

#include <vipir/IR/Instruction/RetInst.h>

namespace parser
{
    ReturnStatement::ReturnStatement(ASTNodePtr&& returnValue)
        : mReturnValue(std::move(returnValue))
    {
    }

    void ReturnStatement::typeCheck(Scope* scope, diagnostic::Diagnostics& diag)
    {
        if (mReturnValue->getType() != scope->currentReturnType)
        {
            diag.compilerError(mReturnValue->getDebugToken().getStart(), mReturnValue->getDebugToken().getEnd(), std::format("Return value of type '{}{}{}' is incompatible with function with return type '{}{}{}'",
                fmt::bold, mReturnValue->getType()->getName(), fmt::defaults,
                fmt::bold, scope->currentReturnType->getName(), fmt::defaults));
        }
        mReturnValue->typeCheck(scope, diag);
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