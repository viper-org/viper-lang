// Copyright 2024 solar-mist

#include "parser/ast/statement/IfStatement.h"

#include <vipir/IR/Instruction/RetInst.h>

#include <vipir/IR/BasicBlock.h>

namespace parser
{
    IfStatement::IfStatement(ASTNodePtr&& condition, ASTNodePtr&& body, ASTNodePtr&& elseBody)
        : mCondition(std::move(condition))
        , mBody(std::move(body))
        , mElseBody(std::move(elseBody))
    {
    }

    void IfStatement::typeCheck(Scope* scope, diagnostic::Diagnostics& diag)
    {
        if (!mCondition->getType()->isBooleanType())
        {
            diag.compilerError(mCondition->getDebugToken().getStart(), mCondition->getDebugToken().getEnd(), std::format("If-statement condition must have type '{}bool{}'",
                fmt::bold, fmt::defaults));
        }
        mCondition->typeCheck(scope, diag);
        mBody->typeCheck(scope, diag);
        if (mElseBody)
            mElseBody->typeCheck(scope, diag);
    }

    vipir::Value* IfStatement::emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope, diagnostic::Diagnostics& diag)
    {
        vipir::Value* condition = mCondition->emit(builder, module, scope, diag);

        vipir::BasicBlock* trueBasicBlock = vipir::BasicBlock::Create("", builder.getInsertPoint()->getParent());
        vipir::BasicBlock* falseBasicBlock;
        vipir::BasicBlock* mergeBasicBlock = vipir::BasicBlock::Create("", builder.getInsertPoint()->getParent());
        if (mElseBody)
        {
            falseBasicBlock = vipir::BasicBlock::Create("", builder.getInsertPoint()->getParent());
            falseBasicBlock->loopEnd() = mergeBasicBlock;
        }

        trueBasicBlock->loopEnd() = mergeBasicBlock;

        if (mElseBody)
        {
            builder.CreateCondBr(condition, trueBasicBlock, falseBasicBlock);
        }
        else
        {
            builder.CreateCondBr(condition, trueBasicBlock, mergeBasicBlock);
        }

        builder.setInsertPoint(trueBasicBlock);
        mBody->emit(builder, module, scope, diag);
        builder.CreateBr(mergeBasicBlock);

        if (mElseBody)
        {
            builder.setInsertPoint(falseBasicBlock);
            mElseBody->emit(builder, module, scope, diag);
            builder.CreateBr(mergeBasicBlock);
        }

        builder.setInsertPoint(mergeBasicBlock);

        return nullptr;
    }

}