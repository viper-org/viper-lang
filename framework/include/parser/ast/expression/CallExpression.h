// Copyright 2024 solar-mist


#ifndef VIPER_FRAMEWORK_PARSER_AST_EXPRESSION_CALL_EXPRESSION_H
#define VIPER_FRAMEWORK_PARSER_AST_EXPRESSION_CALL_EXPRESSION_H 1

#include "parser/ast/Node.h"

namespace parser
{
    class CallExpression : public ASTNode
    {
    public:
        CallExpression(ASTNodePtr function, std::vector<ASTNodePtr> parameters);

        vipir::Value* emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope) override;

    private:
        ASTNodePtr mFunction;
        std::vector<ASTNodePtr> mParameters;
    };

    using CallExpressionPtr = std::unique_ptr<CallExpression>;
}

#endif // VIPER_FRAMEWORK_PARSER_AST_EXPRESSION_CALL_EXPRESSION_H