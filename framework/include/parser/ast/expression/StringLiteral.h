// Copyright 2024 solar-mist

#ifndef VIPER_FRAMEWORK_PARSER_AST_EXPRESSION_STRING_LITERAL_H
#define VIPER_FRAMEWORK_PARSER_AST_EXPRESSION_STRING_LITERAL_H 1

#include "parser/ast/Node.h"

namespace parser
{
    class StringLiteral : public ASTNode
    {
    public:
        StringLiteral(std::string value);

        vipir::Value* emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope) override;

    private:
        std::string mValue;
    };
    using StringLiteralPtr = std::unique_ptr<StringLiteral>;
}

#endif // VIPER_FRAMEWORK_PARSER_AST_EXPRESSION_STRING_LITERAL_H