// Copyright 2023 solar-mist


#ifndef VIPER_FRAMEWORK_PARSER_AST_EXPRESSION_CALL_H
#define VIPER_FRAMEWORK_PARSER_AST_EXPRESSION_CALL_H 1

#include "parser/ast/Node.h"

namespace parsing
{
    class CallExpression : public ASTNode
    {
    public:
        CallExpression(ASTNodePtr callee, std::vector<ASTNodePtr> parameters);

        vipir::Value* emit(vipir::Builder& builder, vipir::Module& module, Environment* scope) override;

    private:
        ASTNodePtr mCallee;
        std::vector<ASTNodePtr> mParameters;
    };

    using CallExpressionPtr = std::unique_ptr<CallExpression>;
}

#endif