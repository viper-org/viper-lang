// Copyright 2024 solar-mist

#include "parser/ast/statement/ReturnStatement.h"

#include <vipir/IR/Instruction/RetInst.h>

namespace parser
{
    ReturnStatement::ReturnStatement(Scope* scope, ASTNodePtr returnValue)
        : ASTNode(scope)
        , mReturnValue(std::move(returnValue))
    {
    }

    vipir::Value* ReturnStatement::codegen(vipir::IRBuilder& builder, vipir::Module& module, diagnostic::Diagnostics& diag)
    {
        return builder.CreateRet(mReturnValue->codegen(builder, module, diag));
    }
}