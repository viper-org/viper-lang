#include "ssa/value/instruction/alloca.hh"
#include <ssa/value/global/function.hh>

namespace SSA
{
    Function* Function::Create(Module& module, const std::string& name)
    {
        Function* func = new Function(module, name);
        
        return func;
    }

    Function::Function(Module& module, const std::string& name)
        :Value(module), _name(name)
    {
    }

    std::vector<BasicBlock*>& Function::GetBasicBlockList()
    {
        return _basicBlockList;
    }

    std::vector<AllocaInst*>& Function::GetAllocaList()
    {
        return _allocaList;
    }

    void Function::Print(std::ostream& stream, int indent) const
    {
        stream << std::string(indent, ' ') << "define int32 " << _name << "() {\n";
        for(BasicBlock* bb : _basicBlockList)
            bb->Print(stream, indent + 2);
        stream << std::string(indent, ' ') << "}";
    }

    std::string Function::GetID() const
    {
        return _name;
    }

    Codegen::Value* Function::Emit(Codegen::Assembly& assembly)
    {
        assembly.CreateGlobal(_name);
        assembly.CreateLabel(_name);

        for(BasicBlock* bb : _basicBlockList)
            bb->Emit(assembly);

        return nullptr;
    }

    void Function::Dispose()
    {
        for(BasicBlock* basicBlock : _basicBlockList)
            basicBlock->Dispose();

        delete this;
    }

    void Function::SortAllocas()
    {
        int offset = 0;
        for(AllocaInst* alloca : _allocaList)
        {
            offset -= 4; // TODO: Calculate proper size and offset
            alloca->_offset = offset;
        }
    }
}