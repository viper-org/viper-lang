// Copyright 2024 solar-mist

#include "parser/ast/expression/CallExpression.h"

#include "type/FunctionType.h"

#include <vipir/Module.h>

#include <vipir/IR/Function.h>
#include <vipir/IR/Instruction/CallInst.h>

namespace parser
{
    CallExpression::CallExpression(Scope* scope, ASTNodePtr callee)
        : ASTNode(scope, callee->getErrorToken())
        , mCallee(std::move(callee))
    {
    }

    vipir::Value* CallExpression::codegen(vipir::IRBuilder& builder, vipir::Module& module, diagnostic::Diagnostics& diag)
    {
        auto callee = mCallee->codegen(builder, module, diag);
        return builder.CreateCall(static_cast<vipir::Function*>(callee), {});
    }

    void CallExpression::typeCheck(diagnostic::Diagnostics& diag, bool& exit)
    {
        mCallee->typeCheck(diag, exit);

        if (!mCallee->getType()->isFunctionType())
        {
            diag.reportCompilerError(
                mCallee->getErrorToken().getStartLocation(),
                mCallee->getErrorToken().getEndLocation(),
                std::format("value of type '{}{}{}' cannot be called",
                    fmt::bold, mCallee->getType()->getName(), fmt::defaults)
            );
            exit = true;
        }

        mType = static_cast<FunctionType*>(mCallee->getType())->getReturnType();
    }

    bool CallExpression::triviallyImplicitCast(diagnostic::Diagnostics&, Type*)
    {
        return false;
    }
}