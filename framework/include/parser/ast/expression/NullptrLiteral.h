// Copyright 2024 solar-mist

#ifndef VIPER_FRAMEWORK_PARSER_AST_EXPRESSION_NULLPTR_LITERAL_H
#define VIPER_FRAMEWORK_PARSER_AST_EXPRESSION_NULLPTR_LITERAL_H 1

#include "parser/ast/Node.h"

namespace parser
{
    class NullptrLiteral : public ASTNode
    {
    public:
        NullptrLiteral(Type* type);

        vipir::Value* emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope, diagnostic::Diagnostics& diag) override;
    };
    using NullptrLiteralPtr = std::unique_ptr<NullptrLiteral>;
}

#endif // VIPER_FRAMEWORK_PARSER_AST_EXPRESSION_NULLPTR_LITERAL_H