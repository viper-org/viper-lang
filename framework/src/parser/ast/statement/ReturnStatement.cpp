// Copyright 2023 solar-mist


#include "parser/ast/statement/ReturnStatement.h"

#include <vipir/IR/Instruction/RetInst.h>

namespace parsing
{
    ReturnStatement::ReturnStatement(ASTNodePtr value)
        : mValue(std::move(value))
    {
    }

    vipir::Value* ReturnStatement::emit(vipir::Builder& builder, vipir::Module& module, Environment* scope)
    {
        if (mValue)
        {
            vipir::Value* returnValue = mValue->emit(builder, module, scope);

            return builder.CreateRet(returnValue);
        }
        return builder.CreateRet(nullptr);
    }
}