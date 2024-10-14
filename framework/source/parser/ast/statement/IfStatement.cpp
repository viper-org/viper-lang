// Copyright 2024 solar-mist

#include "parser/ast/statement/IfStatement.h"

#include <vipir/IR/BasicBlock.h>

namespace parser
{
    IfStatement::IfStatement(Scope* scope, ASTNodePtr&& condition, ASTNodePtr&& body, ASTNodePtr&& elseBody, lexer::Token token)
        : ASTNode(scope, Type::Get("void"), token)
        , mCondition(std::move(condition))
        , mBody(std::move(body))
        , mElseBody(std::move(elseBody))
    {
    }

    vipir::Value* IfStatement::codegen(vipir::IRBuilder& builder, vipir::Module& module, diagnostic::Diagnostics& diag)
    {
        vipir::Value* condition = mCondition->codegen(builder, module, diag);

        vipir::BasicBlock* trueBasicBlock = vipir::BasicBlock::Create("", builder.getInsertPoint()->getParent());
        vipir::BasicBlock* falseBasicBlock;
        if (mElseBody)
        {
            falseBasicBlock = vipir::BasicBlock::Create("", builder.getInsertPoint()->getParent());
        }

        vipir::BasicBlock* mergeBasicBlock = vipir::BasicBlock::Create("", builder.getInsertPoint()->getParent());

        if (mElseBody)
        {
            builder.CreateCondBr(condition, trueBasicBlock, falseBasicBlock);
        }
        else
        {
            builder.CreateCondBr(condition, trueBasicBlock, mergeBasicBlock);
        }

        builder.setInsertPoint(trueBasicBlock);
        mBody->codegen(builder, module, diag);
        builder.CreateBr(mergeBasicBlock);

        if (mElseBody)
        {
            builder.setInsertPoint(falseBasicBlock);
            mElseBody->codegen(builder, module, diag);
            builder.CreateBr(mergeBasicBlock);
        }

        builder.setInsertPoint(mergeBasicBlock);

        return nullptr;
    }

    void IfStatement::typeCheck(diagnostic::Diagnostics& diag, bool& exit)
    {
        mCondition->typeCheck(diag, exit);
        mBody->typeCheck(diag, exit);
        if (mElseBody)
        {
            mElseBody->typeCheck(diag, exit);
        }

        if (!mCondition->getType()->isBooleanType())
        {
            auto boolType = Type::Get("bool");

            if (mCondition->implicitCast(diag, boolType))
            {
                mCondition = Cast(mCondition, boolType);
            }
            else
            {
                diag.reportCompilerError(
                    mCondition->getErrorToken().getStartLocation(),
                    mCondition->getErrorToken().getEndLocation(),
                    std::format("value of type '{}{}{}' cannot be used as a condition in if-statement",
                        fmt::bold, mCondition->getType()->getName(), fmt::defaults)
                );
            }
        }
    }

    bool IfStatement::triviallyImplicitCast(diagnostic::Diagnostics&, Type*)
    {
        return false;
    }
}