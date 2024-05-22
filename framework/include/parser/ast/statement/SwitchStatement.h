#ifndef VIPER_SWITCHSTATEMENT_H
#define VIPER_SWITCHSTATEMENT_H

#include "parser/ast/Node.h"

namespace parser
{
    struct SwitchSection
    {
        ASTNodePtr label;
        std::vector<ASTNodePtr> body;
    };

    class SwitchStatement : public ASTNode
    {
    public:
        SwitchStatement(ASTNodePtr&& value, std::vector<SwitchSection>&& cases, Scope* scope);

        void typeCheck(Scope* scope, diagnostic::Diagnostics& diag) override;
        vipir::Value* emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope, diagnostic::Diagnostics& diag) override;

    private:
        ASTNodePtr mValue;
        std::vector<SwitchSection> mSections;
        ScopePtr mScope;
    };

    using SwitchStatementPtr = std::unique_ptr<SwitchStatement>;
}

#endif //VIPER_SWITCHSTATEMENT_H
