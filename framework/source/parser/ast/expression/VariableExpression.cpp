// Copyright 2024 solar-mist

#include "parser/ast/expression/VariableExpression.h"

#include <vipir/IR/Function.h>
#include <vipir/IR/Instruction/LoadInst.h>

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
        if (dynamic_cast<vipir::Function*>(symbol->value)) return symbol->value;
        
        return builder.CreateLoad(symbol->value);
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
            std::exit(EXIT_FAILURE);
        }
        mType = symbol->type;
    }

    bool VariableExpression::triviallyImplicitCast(diagnostic::Diagnostics& diag, Type* destType)
    {
        return false;
    }
}