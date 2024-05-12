// Copyright 2024 solar-mist


#ifndef VIPER_FRAMEWORK_PARSER_AST_EXPRESSION_SCOPE_RESOLUTION_H
#define VIPER_FRAMEWORK_PARSER_AST_EXPRESSION_SCOPE_RESOLUTION_H 1

#include "parser/ast/Node.h"

#include "lexer/Token.h"

namespace parser
{
    class ScopeResolution : public ASTNode
    {
    public:
        ScopeResolution(ASTNodePtr left, lexing::Token token, ASTNodePtr right);

        std::vector<std::string> getNames();

        vipir::Value* emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope, diagnostic::Diagnostics& diag) override;

    private:
        ASTNodePtr mLeft;
        lexing::Token mToken;
        ASTNodePtr mRight;
    };

    using ScopeResolutionPtr = std::unique_ptr<ScopeResolution>;
}

#endif // VIPER_FRAMEWORK_PARSER_AST_EXPRESSION_SCOPE_RESOLUTION_H