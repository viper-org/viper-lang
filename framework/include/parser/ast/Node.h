// Copyright 2023 solar-mist


#ifndef VIPER_FRAMEWORK_PARSER_AST_NODE_H
#define VIPER_FRAMEWORK_PARSER_AST_NODE_H 1

#include <memory>

namespace parsing
{
    class ASTNode
    {
    public:
        virtual ~ASTNode() { }
    };
    using ASTNodePtr = std::unique_ptr<ASTNode>;
}

#endif