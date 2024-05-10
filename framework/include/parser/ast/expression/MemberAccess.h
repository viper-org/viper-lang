// Copyright 2024 solar-mist


#ifndef VIPER_FRAMEWORK_PARSER_AST_EXPRESSION_MEMBER_ACCESS_H
#define VIPER_FRAMEWORK_PARSER_AST_EXPRESSION_MEMBER_ACCESS_H 1

#include "parser/ast/Node.h"

#include "lexer/Token.h"

namespace parser
{
    class MemberAccess : public ASTNode
    {
    friend class CallExpression;
    public:
        MemberAccess(ASTNodePtr struc, std::string field, bool pointer, lexing::Token fieldToken);

        vipir::Value* emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope, diagnostic::Diagnostics& diag) override;

    private:
        ASTNodePtr mStruct;
        std::string mField;
        bool mPointer;
        lexing::Token mFieldToken;
    };

    using MemberAccessPtr = std::unique_ptr<MemberAccess>;
}

#endif // VIPER_FRAMEWORK_PARSER_AST_EXPRESSION_MEMBER_ACCESS_H
