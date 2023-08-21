// Copyright 2023 solar-mist


#ifndef VIPER_FRAMEWORK_PARSER_AST_STATEMENT_IF_H
#define VIPER_FRAMEWORK_PARSER_AST_STATEMENT_IF_H 1

#include "parser/ast/Node.h"

namespace parsing
{
    class IfStatement : public ASTNode
    {
    public:
        IfStatement(ASTNodePtr condition, ASTNodePtr body, ASTNodePtr elseBody);

        vipir::Value* emit(vipir::Builder& builder, vipir::Module& module, Environment* scope) override;

    private:
        ASTNodePtr mCondition;
        ASTNodePtr mBody;
        ASTNodePtr mElseBody;
    };

    using IfStatementPtr = std::unique_ptr<IfStatement>;
}

#endif