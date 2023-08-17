// Copyright 2023 solar-mist


#ifndef VIPER_FRAMEWORK_PARSER_AST_EXPRESSION_INTEGER_H
#define VIPER_FRAMEWORK_PARSER_AST_EXPRESSION_INTEGER_H 1

#include "parser/ast/Node.h"

namespace parsing
{
    class IntegerLiteral : public ASTNode
    {
    public:
        IntegerLiteral(long long value);

        vipir::Value* emit(vipir::Builder& builder, vipir::Module& module) override;

    private:
        long long mValue;
    };

    using IntegerLiteralPtr = std::unique_ptr<IntegerLiteral>;
}

#endif