// Copyright 2024 solar-mist


#include "parser/ast/expression/ScopeResolution.h"
#include "parser/ast/expression/VariableExpression.h"

namespace parser
{
    ScopeResolution::ScopeResolution(ASTNodePtr left, lexing::Token token, ASTNodePtr right)
        : mLeft(std::move(left))
        , mToken(std::move(token))
        , mRight(std::move(right))
    {
    }

    std::vector<std::string> ScopeResolution::getNames()
    {
        std::vector<std::string> ret;
        
        if (auto var = dynamic_cast<VariableExpression*>(mLeft.get()))
        {
            ret.push_back(var->getName());
        }
        if (auto var = dynamic_cast<VariableExpression*>(mRight.get()))
        {
            ret.push_back(var->getName());
        }
        else if (auto scope = dynamic_cast<ScopeResolution*>(mRight.get()))
        {
            auto names = scope->getNames();
            std::move(names.begin(), names.end(), std::back_inserter(ret));
        }

        return ret;
    }

    vipir::Value* ScopeResolution::emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope, diagnostic::Diagnostics& diag)
    {
        return nullptr; // Should never be called
    }
}