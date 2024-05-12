#include "parser/ast/statement/ContinueStatement.h"

namespace parser
{
    ContinueStatement::ContinueStatement(lexing::Token token)
        : mToken(std::move(token))
    {
    }

    vipir::Value* ContinueStatement::emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope, diagnostic::Diagnostics& diag)
    {
        vipir::BasicBlock* continueTo = scope->findContinueBB();

        if (!continueTo)
        {
            diag.compilerError(mToken.getStart(), mToken.getEnd(), "continue statement not within loop");
        }

        builder.CreateBr(continueTo);

        return nullptr;
    }
}