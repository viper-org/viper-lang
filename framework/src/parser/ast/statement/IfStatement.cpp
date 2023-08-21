// Copyright 2023 solar-mist


#include "parser/ast/statement/IfStatement.h"

#include <vipir/IR/BasicBlock.h>

namespace parsing
{
    IfStatement::IfStatement(ASTNodePtr condition, ASTNodePtr body, ASTNodePtr elseBody)
        : mCondition(std::move(condition))
        , mBody(std::move(body))
        , mElseBody(std::move(elseBody))
    {
    }

    vipir::Value* IfStatement::emit(vipir::Builder& builder, vipir::Module& module, Environment* scope)
    {
        vipir::Value* condition = mCondition->emit(builder, module, scope);
        vipir::BasicBlock* trueBasicBlock = vipir::BasicBlock::Create("", builder.getInsertPoint()->getParent());
        vipir::BasicBlock* elseBasicBlock;
        if (mElseBody)
        {
            elseBasicBlock = vipir::BasicBlock::Create("", builder.getInsertPoint()->getParent());
        }

        vipir::BasicBlock* mergeBasicBlock = vipir::BasicBlock::Create("", builder.getInsertPoint()->getParent());

        if (mElseBody)
        {
            builder.CreateCondBr(condition, trueBasicBlock, elseBasicBlock);
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
            builder.setInsertPoint(elseBasicBlock);
            mElseBody->emit(builder, module, scope);
            builder.CreateBr(mergeBasicBlock);
        }

        builder.setInsertPoint(mergeBasicBlock);

        return nullptr;
    }
}