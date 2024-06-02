#ifndef VIPER_FRAMEWORK_PARSER_AST_STATEMENT_CONSTEXPR_STATEMENT_H
#define VIPER_FRAMEWORK_PARSER_AST_STATEMENT_CONSTEXPR_STATEMENT_H

#include "lexer/Token.h"
#include "parser/ast/Node.h"

namespace parser
{
    class ConstexprStatement : public ASTNode
    {
    public:
        ConstexprStatement(Type* type, std::vector<std::string> names, ASTNodePtr&& value, lexing::Token token, bool global);

        void typeCheck(Scope* scope, diagnostic::Diagnostics& diag) override;
        vipir::Value* emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope, diagnostic::Diagnostics& diag) override;

    private:
        std::vector<std::string> mNames;
        ASTNodePtr mValue;
        lexing::Token mToken;
        bool mGlobal;
    };

    using ConstexprStatementPtr = std::unique_ptr<ConstexprStatement>;
}

#endif //VIPER_FRAMEWORK_PARSER_AST_STATEMENT_CONSTEXPR_STATEMENT_H