#ifndef VIPER_FRAMEWORK_PARSER_AST_STATEMENT_CONSTEXPR_STATEMENT_H
#define VIPER_FRAMEWORK_PARSER_AST_STATEMENT_CONSTEXPR_STATEMENT_H

#include "parser/ast/Node.h"

namespace parser
{
    class ConstexprStatement : public ASTNode
    {
    public:
        ConstexprStatement(Type* type, std::string&& name, ASTNodePtr&& value);

        vipir::Value* emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope, diagnostic::Diagnostics& diag) override;

    private:
        std::string mName;
        ASTNodePtr mValue;
    };

    using ConstexprStatementPtr = std::unique_ptr<ConstexprStatement>;
}

#endif //VIPER_FRAMEWORK_PARSER_AST_STATEMENT_CONSTEXPR_STATEMENT_H