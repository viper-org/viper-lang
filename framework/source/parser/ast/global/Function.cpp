// Copyright 2024 solar-mist

#include "parser/ast/global/Function.h"

#include <vipir/IR/Function.h>

namespace parser
{
    Function::Function(std::string name, std::vector<ASTNodePtr> body, ScopePtr scope)
        : ASTNode(scope.get())
        , mName(std::move(name))
        , mBody(std::move(body))
        , mOwnScope(std::move(scope))
    {
    }

    vipir::Value* Function::codegen(vipir::IRBuilder& builder, vipir::Module& module, diagnostic::Diagnostics& diag)
    {
        auto functionType = vipir::FunctionType::Create(vipir::Type::GetIntegerType(32), {});
        auto function = vipir::Function::Create(functionType, module, mName);

        auto entryBB = vipir::BasicBlock::Create("", function);
        builder.setInsertPoint(entryBB);

        for (auto& node : mBody)
        {
            node->codegen(builder, module, diag);
        }

        return nullptr;
    }
}