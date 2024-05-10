// Copyright 2024 solar-mist

#ifndef VIPER_FRAMEWORK_PARSER_AST_EXPRESSION_STRUCT_INITIALIZER_H
#define VIPER_FRAMEWORK_PARSER_AST_EXPRESSION_STRUCT_INITIALIZER_H 1

#include "parser/ast/Node.h"

namespace parser
{
    class StructInitializer : public ASTNode
    {
    public:
        StructInitializer(Type* type, std::vector<ASTNodePtr>&& body);

        vipir::Value* emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope, diagnostic::Diagnostics& diag) override;

    private:
        std::vector<ASTNodePtr> mBody;
    };
    using StructInitializerPtr = std::unique_ptr<StructInitializer>;
}

#endif // VIPER_FRAMEWORK_PARSER_AST_EXPRESSION_STRUCT_INITIALIZER_H