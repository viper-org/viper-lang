// Copyright 2024 solar-mist

#include "parser/ast/global/Function.h"

#include <vipir/IR/Function.h>

namespace parser
{
    Function::Function(std::string name, FunctionType* type, std::vector<ASTNodePtr> body, ScopePtr scope, lexer::Token token)
        : ASTNode(scope->parent, type, token)
        , mName(std::move(name))
        , mBody(std::move(body))
        , mOwnScope(std::move(scope))
    {
        mScope->symbols.emplace_back(mName, mType);
    }

    vipir::Value* Function::codegen(vipir::IRBuilder& builder, vipir::Module& module, diagnostic::Diagnostics& diag)
    {
        auto functionType = static_cast<vipir::FunctionType*>(mType->getVipirType());
        auto function = vipir::Function::Create(functionType, module, mName);

        mScope->resolveSymbol(mName)->value = function;

        auto entryBB = vipir::BasicBlock::Create("", function);
        builder.setInsertPoint(entryBB);

        for (auto& node : mBody)
        {
            node->codegen(builder, module, diag);
        }

        return nullptr;
    }
    
    void Function::typeCheck(diagnostic::Diagnostics& diag, bool& exit)
    {
        for (auto& node : mBody)
        {
            node->typeCheck(diag, exit);
        }
    }

    bool Function::triviallyImplicitCast(diagnostic::Diagnostics&, Type*)
    {
        return false;
    }
}