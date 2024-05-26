// Copyright 2024 solar-mist

#ifndef VIPER_FRAMEWORK_PARSER_AST_AST_NODE_H
#define VIPER_FRAMEWORK_PARSER_AST_AST_NODE_H 1

#include "symbol/Scope.h"

#include "type/Type.h"

#include "diagnostic/Diagnostic.h"

#include "lexer/Token.h"

#include <vipir/IR/IRBuilder.h>

#include <memory>

namespace parser
{
    class ASTNode
    {
    public:
        ASTNode() { }
        virtual ~ASTNode() { }

        Type* getType() const { return mType; }
        lexing::Token& getDebugToken() { return mPreferredDebugToken; }

        virtual void typeCheck(Scope* scope, diagnostic::Diagnostics& diag) = 0;
        virtual vipir::Value* emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope, diagnostic::Diagnostics& diag) = 0;
    
    protected:
        Type* mType;

        lexing::Token mPreferredDebugToken;
    };
    using ASTNodePtr = std::unique_ptr<ASTNode>;
}

#endif // VIPER_FRAMEWORK_PARSER_AST_AST_NODE_H