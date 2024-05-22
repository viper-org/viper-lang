// Copyright 2024 solar-mist

#include "parser/ast/statement/WhileStatement.h"
#include "parser/ast/expression/BooleanLiteral.h"

#include <vipir/IR/Instruction/RetInst.h>

#include <vipir/IR/BasicBlock.h>

namespace parser
{
    WhileStatement::WhileStatement(ASTNodePtr&& condition, ASTNodePtr&& body, Scope* scope)
        : mCondition(std::move(condition))
        , mBody(std::move(body))
        , mScope(scope)
    {
    }

    void WhileStatement::typeCheck(Scope* scope, diagnostic::Diagnostics& diag)
    {
        if (!mCondition->getType()->isBooleanType())
        {
            diag.compilerError(mCondition->getDebugToken().getStart(), mCondition->getDebugToken().getEnd(), std::format("While-statement condition must have type '{}bool{}'",
                fmt::bold, fmt::defaults));
        }
        mCondition->typeCheck(scope, diag);
        mBody->typeCheck(scope, diag);
    }

    vipir::Value* WhileStatement::emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope, diagnostic::Diagnostics& diag)
    {
        vipir::BasicBlock* conditionBasicBlock = vipir::BasicBlock::Create("", builder.getInsertPoint()->getParent());
        vipir::BasicBlock* bodyBasicBlock = vipir::BasicBlock::Create("", builder.getInsertPoint()->getParent());
        vipir::BasicBlock* doneBasicBlock = vipir::BasicBlock::Create("", builder.getInsertPoint()->getParent());

        conditionBasicBlock->loopEnd() = doneBasicBlock;
        bodyBasicBlock->loopEnd() = doneBasicBlock;

        scope = mScope.get();
        scope->breakTo = doneBasicBlock;
        scope->continueTo = conditionBasicBlock;

        if (auto boolean = dynamic_cast<BooleanLiteral*>(mCondition.get()))
        {
            bodyBasicBlock->loopEnd() = nullptr;
            conditionBasicBlock->loopEnd() = nullptr;
            if (boolean->getValue())
            {
                builder.CreateBr(bodyBasicBlock);
                builder.setInsertPoint(bodyBasicBlock);
                mBody->emit(builder, module, scope, diag);
                builder.CreateBr(bodyBasicBlock);
            }
            else
            {
                builder.CreateBr(doneBasicBlock);
            }
            builder.setInsertPoint(doneBasicBlock);
            return nullptr;
        }

        builder.CreateBr(conditionBasicBlock);
        builder.setInsertPoint(conditionBasicBlock);
        vipir::Value* condition = mCondition->emit(builder, module, scope, diag);
        builder.CreateCondBr(condition, bodyBasicBlock, doneBasicBlock);

        builder.setInsertPoint(bodyBasicBlock);
        mBody->emit(builder, module, scope, diag);
        builder.CreateBr(conditionBasicBlock);

        builder.setInsertPoint(doneBasicBlock);

        return nullptr;
    }
}