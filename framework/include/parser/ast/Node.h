// Copyright 2023 solar-mist


#ifndef VIPER_FRAMEWORK_PARSER_AST_NODE_H
#define VIPER_FRAMEWORK_PARSER_AST_NODE_H 1

#include "type/Type.h"

#include "environment/Environment.h"

#include <vipir/IR/Builder.h>
#include <vipir/IR/Value.h>

#include <memory>

namespace parsing
{
    class ASTNode
    {
    public:
        virtual ~ASTNode() { }

        virtual vipir::Value* emit(vipir::Builder& builder, vipir::Module& module, Environment* scope) = 0;

        void setType(Type* newType) { mType = newType; }
        Type* getType() const { return mType; }

    protected:
        Type* mType;
    };
    using ASTNodePtr = std::unique_ptr<ASTNode>;
}

#endif