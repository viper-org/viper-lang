#include <codegen/value/basicBlock.hh>
#include <codegen/value/global/function.hh>
#include <codegen/value/instruction/ret.hh>
#include <iostream>

namespace Codegen
{
    BasicBlock* BasicBlock::Create(Module& module, const std::string& name, Function* parent)
    {
        BasicBlock* block = new BasicBlock(module, name, parent);
        
        return block;
    }

    const std::pair<std::string, Register*> BasicBlock::Generate(Register*)
    {
        std::string result;
        for(Instruction* instr : _instructions)
        {
            int8_t ret = (int64_t)dynamic_cast<RetInst*>(instr);
            if(ret)
                instr->SetType(_parent->GetReturnType());
            std::pair<std::string, Register*> codegen = instr->Generate();
            result += codegen.first;
            delete instr;
            Register::FreeRegister(codegen.second);
            if(ret)
                break;
        }
        return std::make_pair(result, nullptr);
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