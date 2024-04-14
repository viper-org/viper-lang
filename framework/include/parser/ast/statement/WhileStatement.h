// Copyright 2024 solar-mist

#ifndef VIPER_FRAMEWORK_PARSER_AST_STATEMENT_WHILE_STATEMENT_H
#define VIPER_FRAMEWORK_PARSER_AST_STATEMENT_WHILE_STATEMENT_H 1

#include "parser/ast/Node.h"

namespace parser
{
    class WhileStatement : public ASTNode
    {
    public:
        WhileStatement(ASTNodePtr&& condition, ASTNodePtr&& body);

        vipir::Value* emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope) override;

    private:
        ASTNodePtr mCondition;
        ASTNodePtr mBody;
    };
    using WhileStatementPtr = std::unique_ptr<WhileStatement>;
}

#endif // VIPER_FRAMEWORK_PARSER_AST_STATEMENT_WHILE_STATEMENT_H