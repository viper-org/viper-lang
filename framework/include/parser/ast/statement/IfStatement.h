// Copyright 2024 solar-mist

#ifndef VIPER_FRAMEWORK_PARSER_AST_STATEMENT_IF_STATEMENT_H
#define VIPER_FRAMEWORK_PARSER_AST_STATEMENT_IF_STATEMENT_H 1

#include "parser/ast/Node.h"

namespace parser
{
    class IfStatement : public ASTNode
    {
    public:
        IfStatement(ASTNodePtr&& condition, ASTNodePtr&& body, ASTNodePtr&& elseBody);

        vipir::Value* emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope) override;

    private:
        ASTNodePtr mCondition;
        ASTNodePtr mBody;
        ASTNodePtr mElseBody;
    };
    using IfStatementPtr = std::unique_ptr<IfStatement>;
}

#endif // VIPER_FRAMEWORK_PARSER_AST_STATEMENT_IF_STATEMENT_H