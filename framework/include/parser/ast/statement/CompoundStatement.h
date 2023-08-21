// Copyright 2023 solar-mist


#ifndef VIPER_FRAMEWORK_PARSER_AST_STATEMENT_COMPOUND_H
#define VIPER_FRAMEWORK_PARSER_AST_STATEMENT_COMPOUND_H 1

#include "parser/ast/Node.h"

namespace parsing
{
    class CompoundStatement : public ASTNode
    {
    public:
        CompoundStatement(std::vector<ASTNodePtr> body, Environment* scope);

        vipir::Value* emit(vipir::Builder& builder, vipir::Module& module, Environment* scope) override;

    private:
        std::vector<ASTNodePtr> mBody;
        EnvironmentPtr mScope;
    };

    using CompoundStatementPtr = std::unique_ptr<CompoundStatement>;
}

#endif