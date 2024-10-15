// Copyright 2024 solar-mist

#ifndef VIPER_FRAMEWORK_PARSER_AST_EXPRESSION_CALL_EXPRESSION_H
#define VIPER_FRAMEWORK_PARSER_AST_EXPRESSION_CALL_EXPRESSION_H 1

#include "parser/ast/ASTNode.h"

#include <cstdint>
#include <memory>

namespace parser
{
    // TODO: Add parameters
    class CallExpression : public ASTNode
    {
    public:
        CallExpression(Scope* scope, ASTNodePtr callee, std::vector<ASTNodePtr> parameters);

        virtual vipir::Value* codegen(vipir::IRBuilder& builder, vipir::Module& module, diagnostic::Diagnostics& diag) override;

        virtual void typeCheck(diagnostic::Diagnostics& diag, bool& exit) override;
        virtual bool triviallyImplicitCast(diagnostic::Diagnostics& diag, Type* destType) override;

    private:
        ASTNodePtr mCallee;
        std::vector<ASTNodePtr> mParameters;
        Symbol* mBestViableFunction;
        Symbol mFakeFunction;

        Symbol* getBestViableFunction(diagnostic::Diagnostics& diag);
    };
    using CallExpressionPtr = std::unique_ptr<CallExpression>;
}

#endif // VIPER_FRAMEWORK_PARSER_AST_EXPRESSION_CALL_EXPRESSION_H