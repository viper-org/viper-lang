// Copyright 2023 solar-mist


#ifndef VIPER_FRAMEWORK_PARSER_AST_STATEMENT_RETURN_H
#define VIPER_FRAMEWORK_PARSER_AST_STATEMENT_RETURN_H 1

#include "parser/ast/Node.h"

namespace parsing
{
    class ReturnStatement : public ASTNode
    {
    public:
        ReturnStatement(ASTNodePtr value);

        vipir::Value* emit(vipir::Builder& builder, vipir::Module& module) override;

    private:
        ASTNodePtr mValue;
    };

    using ReturnStatementPtr = std::unique_ptr<ReturnStatement>;
}

#endif