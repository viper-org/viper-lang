// Copyright 2024 solar-mist

#ifndef VIPER_FRAMEWORK_PARSER_AST_EXPRESSION_INTEGER_LITERAL_H
#define VIPER_FRAMEWORK_PARSER_AST_EXPRESSION_INTEGER_LITERAL_H 1

#include "parser/ast/ASTNode.h"

#include <cstdint>
#include <memory>

namespace parser
{
    class IntegerLiteral : public ASTNode
    {
    public:
        IntegerLiteral(std::uintmax_t value);

    private:
        std::uintmax_t mValue;
    };
    using IntegerLiteralPtr = std::unique_ptr<IntegerLiteral>;
}

#endif // VIPER_FRAMEWORK_PARSER_AST_EXPRESSION_INTEGER_LITERAL_H