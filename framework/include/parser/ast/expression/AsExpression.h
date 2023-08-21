// Copyright 2023 solar-mist


#ifndef VIPER_FRAMEWORK_PARSER_AST_EXPRESSION_AS_H
#define VIPER_FRAMEWORK_PARSER_AST_EXPRESSION_AS_H 1

#include "parser/ast/Node.h"

namespace parsing
{
    class AsExpression : public ASTNode
    {
    public:
        AsExpression(ASTNodePtr source, Type* destination);

        vipir::Value* emit(vipir::Builder& builder, vipir::Module& module, Environment* scope) override;

    private:
        ASTNodePtr mSource;
        Type* mDestination;
    };

    using AsExpressionPtr = std::unique_ptr<AsExpression>;
}

#endif