#ifndef VIPER_CONTINUESTATEMENT_H
#define VIPER_CONTINUESTATEMENT_H

#include "parser/ast/Node.h"

#include "lexer/Token.h"

namespace parser
{
    class ContinueStatement : public ASTNode
    {
    public:
        ContinueStatement(lexing::Token token);

        void typeCheck(Scope* scope, diagnostic::Diagnostics& diag) override;
        vipir::Value* emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope, diagnostic::Diagnostics& diag) override;

    private:
        lexing::Token mToken;
    };
    using ContinueStatementPtr = std::unique_ptr<ContinueStatement>;
}

#endif //VIPER_CONTINUESTATEMENT_H
