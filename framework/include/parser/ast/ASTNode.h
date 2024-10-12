// Copyright 2024 solar-mist

#ifndef VIPER_FRAMEWORK_PARSER_AST_AST_NODE_H
#define VIPER_FRAMEWORK_PARSER_AST_AST_NODE_H 1

#include "symbol/Scope.h"

#include "diagnostic/Diagnostic.h"

#include <vipir/IR/IRBuilder.h>

#include <memory>

namespace parser
{
    class ASTNode
    {
    public:
        ASTNode(Scope* scope) : mScope(scope) { }
        virtual ~ASTNode() { }

        Scope* getScope() const { return mScope; }

        virtual vipir::Value* codegen(vipir::IRBuilder& builder, vipir::Module& module, diagnostic::Diagnostics& diag) = 0;

    protected:
        Scope* mScope;
    };
    using ASTNodePtr = std::unique_ptr<ASTNode>;
}

#endif // VIPER_FRAMEWORK_PARSER_AST_AST_NODE_H