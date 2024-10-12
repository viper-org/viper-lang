// Copyright 2024 solar-mist

#ifndef VIPER_FRAMEWORK_PARSER_AST_AST_NODE_H
#define VIPER_FRAMEWORK_PARSER_AST_AST_NODE_H 1

#include "symbol/Scope.h"

#include <memory>

namespace parser
{
    class ASTNode
    {
    public:
        ASTNode() { }
        virtual ~ASTNode() { }

        Scope* getScope() const { return mScope; }

    protected:
        Scope* mScope;
    };
    using ASTNodePtr = std::unique_ptr<ASTNode>;
}

#endif // VIPER_FRAMEWORK_PARSER_AST_AST_NODE_H