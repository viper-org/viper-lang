// Copyright 2024 solar-mist

#ifndef VIPER_FRAMEWORK_PARSER_AST_EXPRESSION_INTEGER_LITERAL_H
#define VIPER_FRAMEWORK_PARSER_AST_EXPRESSION_INTEGER_LITERAL_H 1

#include "parser/ast/Node.h"

namespace parser
{
    class IntegerLiteral : public ASTNode
    {
    public:
        IntegerLiteral(intmax_t value, Type* type);

        vipir::Value* emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope) override;

    private:
        intmax_t mValue;
    };
    using IntegerLiteralPtr = std::unique_ptr<IntegerLiteral>;
}

#endif // VIPER_FRAMEWORK_PARSER_AST_EXPRESSION_INTEGER_LITERAL_H