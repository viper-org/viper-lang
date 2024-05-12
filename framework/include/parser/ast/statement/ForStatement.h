#ifndef VIPER_FORSTATEMENT_H
#define VIPER_FORSTATEMENT_H

#include "parser/ast/Node.h"

namespace parser
{
    class ForStatement : public ASTNode
    {
    public:
        ForStatement(ASTNodePtr&& init, ASTNodePtr&& condition, std::vector<ASTNodePtr>&& loopExpr, ASTNodePtr&& body, Scope* scope);

        vipir::Value* emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope, diagnostic::Diagnostics& diag) override;

    private:
        ASTNodePtr mInit;
        ASTNodePtr mCondition;
        std::vector<ASTNodePtr> mLoopExpr;
        ASTNodePtr mBody;
        ScopePtr mScope;
    };

    using ForStatementPtr = std::unique_ptr<ForStatement>;
}

#endif //VIPER_FORSTATEMENT_H
