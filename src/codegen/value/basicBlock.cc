#include <codegen/value/basicBlock.hh>
#include <codegen/value/global/function.hh>
#include <iostream>

namespace Codegen
{
    BasicBlock* BasicBlock::Create(Module& module, const std::string& name, Function* parent)
    {
        BasicBlock* block = new BasicBlock(module, name, parent);
        
        return block;
    }

    const std::string BasicBlock::Generate()
    {
        std::string result;
        for(Instruction* instr : _instructions)
        {
            result += instr->Generate();
            delete instr;
        }
        return result;
    }

    BasicBlock::BasicBlock(Module& module, const std::string& name, Function* parent)
        :Value(module), _module(module), _name(name), _parent(parent)
    {
        if(parent)
            parent->GetBasicBlockList().push_back(this);
    }

    std::vector<Instruction*>& BasicBlock::GetInstrList()
    {
        return _instructions;
    }

    Function* BasicBlock::GetParent() const
    {
        return _parent;
    }

    std::string BasicBlock::GetName() const
    {
        return _name;
    }

    Module& BasicBlock::GetModule() const
    {
        return _module;
    }
}