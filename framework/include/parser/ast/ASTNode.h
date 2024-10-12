// Copyright 2024 solar-mist

#ifndef VIPER_FRAMEWORK_PARSER_AST_AST_NODE_H
#define VIPER_FRAMEWORK_PARSER_AST_AST_NODE_H 1

#include "symbol/Scope.h"

#include "type/Type.h"

#include "lexer/Token.h"

#include "diagnostic/Diagnostic.h"

#include <vipir/IR/IRBuilder.h>

#include <memory>

namespace parser
{
    class ASTNode
    {
    public:
        ASTNode(Scope* scope, lexer::Token errorToken) : mScope(scope), mErrorToken(errorToken) { }
        ASTNode(Scope* scope, Type* type, lexer::Token errorToken) : mScope(scope), mType(type), mErrorToken(errorToken) { }
        virtual ~ASTNode() { }

        Scope* getScope() const { return mScope; }
        Type* getType() const { return mType; }
        const lexer::Token& getErrorToken() const { return mErrorToken; }

        virtual vipir::Value* codegen(vipir::IRBuilder& builder, vipir::Module& module, diagnostic::Diagnostics& diag) = 0;

        virtual void typeCheck(diagnostic::Diagnostics& diag, bool& exit) = 0;
        virtual bool implicitCast(diagnostic::Diagnostics& diag, Type* destType) = 0;
        virtual bool triviallyImplicitCast(diagnostic::Diagnostics& diag, Type* destType) = 0;

    protected:
        Scope* mScope;
        Type* mType;

        lexer::Token mErrorToken;
    };
    using ASTNodePtr = std::unique_ptr<ASTNode>;
}

#endif // VIPER_FRAMEWORK_PARSER_AST_AST_NODE_H