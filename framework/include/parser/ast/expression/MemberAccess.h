// Copyright 2024 solar-mist


#ifndef VIPER_FRAMEWORK_PARSER_AST_EXPRESSION_MEMBER_ACCESS_H
#define VIPER_FRAMEWORK_PARSER_AST_EXPRESSION_MEMBER_ACCESS_H 1

#include "parser/ast/Node.h"

namespace parser
{
    class MemberAccess : public ASTNode
    {
    public:
        MemberAccess(ASTNodePtr struc, std::string field, bool pointer);

        vipir::Value* emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope) override;

    private:
        ASTNodePtr mStruct;
        std::string mField;
        bool mPointer;
    };

    using MemberAccessPtr = std::unique_ptr<MemberAccess>;
}

#endif // VIPER_FRAMEWORK_PARSER_AST_EXPRESSION_MEMBER_ACCESS_H