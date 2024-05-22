// Copyright 2024 solar-mist

#include "parser/ast/expression/VariableExpression.h"

#include "symbol/Identifier.h"

#include <vipir/IR/Instruction/AllocaInst.h>
#include <vipir/IR/Instruction/LoadInst.h>

namespace parser
{
    VariableExpression::VariableExpression(std::string&& name, Type* type, lexing::Token token)
        : mName(std::move(name))
        , mToken(std::move(token))
    {
        mType = type;
        mPreferredDebugToken = mToken;
    }

    std::string VariableExpression::getName()
    {
        return mName;
    }

    void VariableExpression::typeCheck(Scope* scope, diagnostic::Diagnostics& diag)
    {
    }

    vipir::Value* VariableExpression::emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope, diagnostic::Diagnostics& diag)
    {
        LocalSymbol* local = scope->findVariable(mName);

        std::vector<std::string> symbols = symbol::GetSymbol({mName}, scope->getNamespaces());

        if (local)
        {
            return builder.CreateLoad(local->alloca);
        }
        else
        {
            for (auto& symbol : symbols)
            {
                if (GlobalFunctions.find(symbol) != GlobalFunctions.end())
                {
                    return GlobalFunctions.at(symbol).function;
                }
                else if (GlobalVariables.find(symbol) != GlobalVariables.end())
                {
                    return builder.CreateLoad(GlobalVariables.at(symbol).global);
                }
            }
        }
        diag.compilerError(mToken.getStart(), mToken.getEnd(), std::format("identifier '{}{}{}' undeclared",
            fmt::bold, mName, fmt::defaults));
    }
}