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

    vipir::Value* ForStatement::emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope)
    {
        vipir::BasicBlock* conditionBasicBlock = vipir::BasicBlock::Create("", builder.getInsertPoint()->getParent());
        vipir::BasicBlock* bodyBasicBlock = vipir::BasicBlock::Create("", builder.getInsertPoint()->getParent());
        vipir::BasicBlock* doneBasicBlock = vipir::BasicBlock::Create("", builder.getInsertPoint()->getParent());

        scope = mScope;

        conditionBasicBlock->loopEnd() = doneBasicBlock;
        bodyBasicBlock->loopEnd() = doneBasicBlock;

        if (mInit)
            mInit->emit(builder, module, scope);

        if (!mCondition)
        {
            builder.CreateBr(bodyBasicBlock);
            builder.setInsertPoint(bodyBasicBlock);

            mBody->emit(builder, module, scope);
            for (auto& node : mLoopExpr)
            {
                node->emit(builder, module, scope);
            }

            builder.CreateBr(bodyBasicBlock);

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

                mBody->emit(builder, module, scope);
                for (auto& node : mLoopExpr) {
                    node->emit(builder, module, scope);
                }

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
        for (auto& node : mLoopExpr)
        {
            node->emit(builder, module, scope);
        }

        builder.CreateBr(conditionBasicBlock);

        builder.setInsertPoint(doneBasicBlock);

        return nullptr;
    }
}