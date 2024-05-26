#include "parser/ast/statement/ForStatement.h"

#include "parser/ast/expression/BooleanLiteral.h"

namespace parser
{
    ForStatement::ForStatement(parser::ASTNodePtr&& init, parser::ASTNodePtr&& condition, std::vector<parser::ASTNodePtr>&& loopExpr, parser::ASTNodePtr&& body, Scope* scope)
        : mInit(std::move(init))
        , mCondition(std::move(condition))
        , mLoopExpr(std::move(loopExpr))
        , mBody(std::move(body))
        , mScope(scope)
    {
    }

    void ForStatement::typeCheck(Scope* scope, diagnostic::Diagnostics& diag)
    {
        if (mInit)
            mInit->typeCheck(scope, diag);
        if (mCondition)
        {
            if (!mCondition->getType()->isBooleanType())
            {
                diag.compilerError(mCondition->getDebugToken().getStart(), mCondition->getDebugToken().getEnd(), std::format("For-expression condition must have type '{}bool{}'",
                    fmt::bold, fmt::defaults));
            }
            mCondition->typeCheck(scope, diag);
        }
        for (auto& node : mLoopExpr)
        {
            node->typeCheck(scope, diag);
        }

        mBody->typeCheck(scope, diag);
    }

    vipir::Value* ForStatement::emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope, diagnostic::Diagnostics& diag)
    {
        vipir::BasicBlock* conditionBasicBlock = vipir::BasicBlock::Create("", builder.getInsertPoint()->getParent());
        vipir::BasicBlock* bodyBasicBlock = vipir::BasicBlock::Create("", builder.getInsertPoint()->getParent());
        vipir::BasicBlock* doneBasicBlock = vipir::BasicBlock::Create("", builder.getInsertPoint()->getParent());

        scope = mScope.get();
        scope->breakTo = doneBasicBlock;
        scope->continueTo = conditionBasicBlock;

        conditionBasicBlock->loopEnd() = doneBasicBlock;
        bodyBasicBlock->loopEnd() = doneBasicBlock;

        if (mInit)
            mInit->emit(builder, module, scope, diag);

        if (!mCondition)
        {
            builder.CreateBr(bodyBasicBlock);
            builder.setInsertPoint(bodyBasicBlock);

            mBody->emit(builder, module, scope, diag);
            for (auto& node : mLoopExpr)
            {
                node->emit(builder, module, scope, diag);
            }

            builder.CreateBr(bodyBasicBlock);

            builder.setInsertPoint(doneBasicBlock);

            return nullptr;
        }

        if (auto boolean = dynamic_cast<BooleanLiteral*>(mCondition.get()))
        {
            bodyBasicBlock->loopEnd() = nullptr;
            conditionBasicBlock->loopEnd() = nullptr;
            if (boolean->getValue())
            {
                builder.CreateBr(bodyBasicBlock);
                builder.setInsertPoint(bodyBasicBlock);

                mBody->emit(builder, module, scope, diag);
                for (auto& node : mLoopExpr) {
                    node->emit(builder, module, scope, diag);
                }

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
        for (auto& node : mLoopExpr)
        {
            node->emit(builder, module, scope, diag);
        }

        builder.CreateBr(conditionBasicBlock);

        builder.setInsertPoint(doneBasicBlock);

        return nullptr;
    }
}