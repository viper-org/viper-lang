// Copyright 2024 solar-mist

#include "parser/ast/statement/WhileStatement.h"
#include "parser/ast/expression/BooleanLiteral.h"

#include <vipir/IR/Instruction/RetInst.h>

#include <vipir/IR/BasicBlock.h>

namespace parser
{
    WhileStatement::WhileStatement(ASTNodePtr&& condition, ASTNodePtr&& body)
        : mCondition(std::move(condition))
        , mBody(std::move(body))
    {
    }

    vipir::Value* WhileStatement::emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope)
    {
        vipir::BasicBlock* conditionBasicBlock = vipir::BasicBlock::Create("", builder.getInsertPoint()->getParent());
        vipir::BasicBlock* bodyBasicBlock = vipir::BasicBlock::Create("", builder.getInsertPoint()->getParent());
        vipir::BasicBlock* doneBasicBlock = vipir::BasicBlock::Create("", builder.getInsertPoint()->getParent());

        conditionBasicBlock->loopEnd() = doneBasicBlock;
        bodyBasicBlock->loopEnd() = doneBasicBlock;

        if (auto boolean = dynamic_cast<BooleanLiteral*>(mCondition.get()))
        {
            bodyBasicBlock->loopEnd() = nullptr;
            conditionBasicBlock->loopEnd() = nullptr;
            if (boolean->getValue() == true)
            {
                builder.CreateBr(bodyBasicBlock);
                builder.setInsertPoint(bodyBasicBlock);
                mBody->emit(builder, module, scope);
                builder.CreateBr(bodyBasicBlock);
            }
            else
            {
                builder.CreateBr(doneBasicBlock);
                builder.setInsertPoint(doneBasicBlock);
            }
            return nullptr;
        }

        builder.CreateBr(conditionBasicBlock);
        builder.setInsertPoint(conditionBasicBlock);
        vipir::Value* condition = mCondition->emit(builder, module, scope);
        builder.CreateCondBr(condition, bodyBasicBlock, doneBasicBlock);

        builder.setInsertPoint(bodyBasicBlock);
        mBody->emit(builder, module, scope);
        builder.CreateBr(conditionBasicBlock);

        builder.setInsertPoint(doneBasicBlock);

        return nullptr;
    }
}