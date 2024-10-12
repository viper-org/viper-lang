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
        IntegerLiteral(Scope* scope, std::uintmax_t value);

        virtual vipir::Value* codegen(vipir::IRBuilder& builder, vipir::Module& module, diagnostic::Diagnostics& diag) override;

    private:
        std::uintmax_t mValue;
    };
    using IntegerLiteralPtr = std::unique_ptr<IntegerLiteral>;
}

#endif // VIPER_FRAMEWORK_PARSER_AST_EXPRESSION_INTEGER_LITERAL_H