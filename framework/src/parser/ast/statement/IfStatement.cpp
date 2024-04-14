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

    vipir::Value* IfStatement::emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope)
    {
        vipir::Value* condition = mCondition->emit(builder, module, scope);

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
        mBody->emit(builder, module, scope);
        builder.CreateBr(mergeBasicBlock);

        if (mElseBody)
        {
            builder.setInsertPoint(falseBasicBlock);
            mElseBody->emit(builder, module, scope);
            builder.CreateBr(mergeBasicBlock);
        }

        builder.setInsertPoint(mergeBasicBlock);

        return nullptr;
    }

}