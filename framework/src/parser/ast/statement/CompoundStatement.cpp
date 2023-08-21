// Copyright 2023 solar-mist


#include "parser/ast/statement/CompoundStatement.h"

namespace parsing
{
    CompoundStatement::CompoundStatement(std::vector<ASTNodePtr> body, Environment* scope)
        : mBody(std::move(body))
        , mScope(EnvironmentPtr(scope))
    {
    }

    vipir::Value* CompoundStatement::emit(vipir::Builder& builder, vipir::Module& module, Environment*)
    {
        for (auto& node : mBody)
        {
            node->emit(builder, module, mScope.get());
        }
        
        return nullptr;
    }
}