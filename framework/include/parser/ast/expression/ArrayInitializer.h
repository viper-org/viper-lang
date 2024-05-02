// Copyright 2024 solar-mist

#ifndef VIPER_FRAMEWORK_PARSER_AST_EXPRESSION_ARRAY_INITIALIZER_H
#define VIPER_FRAMEWORK_PARSER_AST_EXPRESSION_ARRAY_INITIALIZER_H 1

#include "parser/ast/Node.h"

namespace parser
{
    class ArrayInitializer : public ASTNode
    {
    public:
        ArrayInitializer(std::vector<ASTNodePtr>&& body);

        vipir::Value* emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope) override;

    private:
        std::vector<ASTNodePtr> mBody;
    };
    using ArrayInitializerPtr = std::unique_ptr<ArrayInitializer>;
}

#endif // VIPER_FRAMEWORK_PARSER_AST_EXPRESSION_ARRAY_INITIALIZER_H