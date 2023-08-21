// Copyright 2023 solar-mist


#include "parser/ast/statement/IfStatement.h"

#include <vipir/IR/BasicBlock.h>

namespace parsing
{
    IfStatement::IfStatement(ASTNodePtr condition, ASTNodePtr body)
        : mCondition(std::move(condition))
        , mBody(std::move(body))
    {
    }

    vipir::Value* IfStatement::emit(vipir::Builder& builder, vipir::Module& module, Environment* scope)
    {
        vipir::Value* condition = mCondition->emit(builder, module, scope);
        vipir::BasicBlock* trueBasicBlock = vipir::BasicBlock::Create("", builder.getInsertPoint()->getParent());
        vipir::BasicBlock* mergeBasicBlock = vipir::BasicBlock::Create("", builder.getInsertPoint()->getParent());

        builder.CreateCondBr(condition, trueBasicBlock, mergeBasicBlock);

        builder.setInsertPoint(trueBasicBlock);
        mBody->emit(builder, module, scope);
        builder.CreateBr(mergeBasicBlock);

        builder.setInsertPoint(mergeBasicBlock);

        return nullptr;
    }
}