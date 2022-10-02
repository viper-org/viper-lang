#include <ssa/value/basicBlock.hh>
#include <ssa/value/global/function.hh>
#include <ssa/value/instruction/return.hh>

namespace SSA
{
    BasicBlock* BasicBlock::Create(Module& module, Function* parent, const std::string& name)
    {
        BasicBlock* bb = new BasicBlock(module, parent, name);
        
        return bb;
    }

    BasicBlock::BasicBlock(Module& module, Function* parent, const std::string& name)
        :Value(module), _name(name), _parent(parent)
    {
        if(_name == "")
        {
            _name = std::to_string(module.GetNextInstName());
        }

        if(parent)
            parent->GetBasicBlockList().push_back(this);
    }

    Function* BasicBlock::GetParent() const
    {
        return _parent;
    }

    std::vector<Instruction*>& BasicBlock::GetInstList()
    {
        return _instList;
    }

    void BasicBlock::Print(std::ostream& stream, int indent) const
    {
        stream << _name << ":";
        stream << "\n";
        for(Instruction* inst : _instList)
        {
            inst->Print(stream, indent);
            if(inst->GetInstType() == InstType::Ret)
                break;
        }
    }

    std::string BasicBlock::GetID() const
    {
        return _name;
    }

    Codegen::Value* BasicBlock::Emit(Codegen::Assembly& assembly)
    {
        for(Instruction* inst : _instList)
        {
            inst->Emit(assembly);
            if(inst->GetInstType() == InstType::Ret)
                break;
        }

        return nullptr;
    }

    void BasicBlock::Dispose()
    {
        for(Instruction* inst : _instList)
            inst->Dispose();
        
        delete this;
    }
}