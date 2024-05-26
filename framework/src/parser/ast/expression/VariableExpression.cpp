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
    }

    std::string VariableExpression::getName()
    {
        return mName;
    }

    vipir::Value* VariableExpression::emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope, diagnostic::Diagnostics& diag)
    {
        LocalSymbol* local = scope->findVariable(mName);

        std::vector<std::string> symbols = symbol::GetSymbol({mName}, scope->getNamespaces());

        if (local)
        {
            if (local->alloca->isConstant()) return local->alloca;

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
                    vipir::Value* value = GlobalVariables[symbol].global;
                    if (value->isConstant()) return value;

                    if (value->getType()->isPointerType()) return builder.CreateLoad(value); // TODO: Something better than this
                }
            }
        }
        diag.compilerError(mToken.getStart(), mToken.getEnd(), std::format("identifier '{}{}{}' undeclared",
            fmt::bold, mName, fmt::defaults));
    }
}