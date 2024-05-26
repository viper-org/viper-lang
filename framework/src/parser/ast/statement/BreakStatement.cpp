// Copyright 2024 solar-mist


#include "parser/ast/statement/BreakStatement.h"

namespace parser
{
    BreakStatement::BreakStatement(lexing::Token token)
        : mToken(std::move(token))
    {
    }

    void BreakStatement::typeCheck(Scope* scope, diagnostic::Diagnostics& diag)
    {
    }

    vipir::Value* BreakStatement::emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope, diagnostic::Diagnostics& diag)
    {
        vipir::BasicBlock* breakTo = scope->findBreakBB();

        if (!breakTo)
        {
            diag.compilerError(mToken.getStart(), mToken.getEnd(), "break statement not within loop");
        }

        builder.CreateBr(breakTo);

        return nullptr;
    }
}