// Copyright 2024 solar-mist

#include "parser/ast/expression/VariableExpression.h"

#include <vipir/IR/Function.h>
#include <vipir/IR/Instruction/LoadInst.h>
#include <vipir/IR/Instruction/AllocaInst.h>

#include <cmath>

namespace parser
{
    VariableExpression::VariableExpression(Scope* scope, std::string name, lexer::Token token)
        : ASTNode(scope, token)
        , mName(std::move(name))
    {
    }

    vipir::Value* VariableExpression::codegen(vipir::IRBuilder& builder, vipir::Module& module, diagnostic::Diagnostics& diag)
    {
        auto symbol = mScope->resolveSymbol(mName);
        if (symbol->type->isFunctionType()) return symbol->getLatestValue();
        
        auto latestValue = symbol->getLatestValue(builder.getInsertPoint());
        if (dynamic_cast<vipir::AllocaInst*>(latestValue)) return builder.CreateLoad(latestValue);
        return latestValue;
    }

    void VariableExpression::semanticCheck(diagnostic::Diagnostics& diag, bool& exit, bool statement)
    {
    }
    
    void VariableExpression::typeCheck(diagnostic::Diagnostics& diag, bool& exit)
    {
        auto symbol = mScope->resolveSymbol(mName);
        if (!symbol)
        {
            diag.reportCompilerError(
                mErrorToken.getStartLocation(),
                mErrorToken.getEndLocation(),
                std::format("undeclared identifier '{}{}{}'",
                    fmt::bold, mName, fmt::defaults)
            );
            exit = true;
            mType = Type::Get("error-type");
        }
        else
        {
            mType = symbol->type;
        }
    }

    bool VariableExpression::triviallyImplicitCast(diagnostic::Diagnostics& diag, Type* destType)
    {
        return false;
    }

    std::string VariableExpression::getName()
    {
        return mName;
    }
}