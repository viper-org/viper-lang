#include "parser/ast/statement/SwitchStatement.h"

#include <vipir/IR/Instruction/BinaryInst.h>

namespace parser
{
    SwitchStatement::SwitchStatement(ASTNodePtr&& value, std::vector<SwitchSection>&& sections)
        : mValue(std::move(value))
        , mSections(std::move(sections))
    {
    }

    vipir::Value* SwitchStatement::emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope, diagnostic::Diagnostics& diag)
    {
        std::vector<vipir::BasicBlock*> conditionBlocks;
        std::vector<vipir::BasicBlock*> bodyBlocks;

        vipir::Value* value = mValue->emit(builder, module, scope, diag);

        if (mSections.empty())
        {
            return nullptr;
        }

        for (auto& sec : mSections)
            conditionBlocks.push_back(vipir::BasicBlock::Create("", builder.getInsertPoint()->getParent()));
        for (auto& sec : mSections)
            bodyBlocks.push_back(vipir::BasicBlock::Create("", builder.getInsertPoint()->getParent()));

        vipir::BasicBlock* endBlock = vipir::BasicBlock::Create("", builder.getInsertPoint()->getParent());
        scope->breakTo = endBlock;

        for (int i = 0; i < mSections.size(); i++)
        {
            auto& sec = mSections[i];
            auto conditionBlock = conditionBlocks[i];
            auto bodyBlock = bodyBlocks[i];
            
            builder.setInsertPoint(conditionBlock);
            if (sec.label)
            {
                vipir::Value* secValue = sec.label->emit(builder, module, scope, diag);
                vipir::Value* condition = builder.CreateCmpEQ(value, secValue);
                vipir::BasicBlock* falseBlock = endBlock;
                if (i < mSections.size() - 1)
                    falseBlock = conditionBlocks[i + 1];
                builder.CreateCondBr(condition, bodyBlock, falseBlock);
            }
            else // Default case
            {
                builder.CreateBr(bodyBlock);
            }

            builder.setInsertPoint(bodyBlock);
            for (auto& node : sec.body)
                node->emit(builder, module, scope, diag);
        }

        builder.setInsertPoint(endBlock);

        return nullptr;
    }
}