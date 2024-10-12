// Copyright 2024 solar-mist

#include "parser/ast/statement/ReturnStatement.h"

#include <vipir/IR/Instruction/RetInst.h>

namespace parser
{
    ReturnStatement::ReturnStatement(Scope* scope, ASTNodePtr returnValue, lexer::Token token)
        : ASTNode(scope, Type::Get("void"), token)
        , mReturnValue(std::move(returnValue))
    {
    }

    vipir::Value* ReturnStatement::codegen(vipir::IRBuilder& builder, vipir::Module& module, diagnostic::Diagnostics& diag)
    {
        vipir::Value* returnValue = nullptr;
        if (mReturnValue)
        {
            returnValue = mReturnValue->codegen(builder, module, diag);
        }

        return builder.CreateRet(returnValue);
    }

    void ReturnStatement::typeCheck(diagnostic::Diagnostics& diag, bool& exit)
    {
        if (mReturnValue)
        {
            mReturnValue->typeCheck(diag, exit);
        }

        auto returnType = mScope->currentReturnType;
        if (returnType->isVoidType())
        {
            if (mReturnValue != nullptr)
            {
                diag.reportCompilerError(
                    mReturnValue->getErrorToken().getStartLocation(),
                    mReturnValue->getErrorToken().getEndLocation(),
                    std::format("value of type '{}{}{}' is not compatible with return type '{}{}{}'",
                        fmt::bold, mReturnValue->getType()->getName(), fmt::defaults,
                        fmt::bold, returnType->getName(), fmt::defaults)
                );
                exit = true;
            }
        }
        else
        {
            if (!mReturnValue)
            {
                diag.reportCompilerError(
                    getErrorToken().getStartLocation(),
                    getErrorToken().getEndLocation(),
                    std::format("non-void function returning '{}{}{}' cannot return '{}void{}'",
                        fmt::bold, returnType->getName(), fmt::defaults,
                        fmt::bold, fmt::defaults)
                );
                exit = true;
            }
            else if (returnType != mReturnValue->getType())
            {
                if (!mReturnValue->implicitCast(diag, returnType))
                {
                    diag.reportCompilerError(
                        mReturnValue->getErrorToken().getStartLocation(),
                        mReturnValue->getErrorToken().getEndLocation(),
                        std::format("value of type '{}{}{}' is not compatible with return type '{}{}{}'",
                            fmt::bold, mReturnValue->getType()->getName(), fmt::defaults,
                            fmt::bold, returnType->getName(), fmt::defaults)
                    );
                    exit = true;
                }
            }
        }
    }

    bool ReturnStatement::implicitCast(diagnostic::Diagnostics&, Type*)
    {
        return false;
    }

    bool ReturnStatement::triviallyImplicitCast(diagnostic::Diagnostics&, Type*)
    {
        return false;
    }
}