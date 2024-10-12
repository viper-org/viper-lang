// Copyright 2024 solar-mist

#ifndef VIPER_FRAMEWORK_PARSER_AST_STATEMENT_RETURN_STATEMENT_H
#define VIPER_FRAMEWORK_PARSER_AST_STATEMENT_RETURN_STATEMENT_H 1

#include "parser/ast/ASTNode.h"

#include <memory>

namespace parser
{
    class ReturnStatement : public ASTNode
    {
    public:
        ReturnStatement(ASTNodePtr returnValue);

    private:
        ASTNodePtr mReturnValue;
    };
    using ReturnStatementPtr = std::unique_ptr<ReturnStatement>;
}

#endif // VIPER_FRAMEWORK_PARSER_AST_STATEMENT_RETURN_STATEMENT_H