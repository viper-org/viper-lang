#include "ssa/value/basicBlock.hh"
#include <ssa/value/global/function.hh>

namespace SSA
{
    Function::Function(Module& module, const std::string& name)
        :Value(module), _name(name)
    {
    }

    std::vector<BasicBlock*>& Function::GetBasicBlockList()
    {
        return _basicBlockList;
    }

    void Function::Print(std::ostream& stream, int indent) const
    {
        stream << std::string(indent, ' ') << "define int32 " << _name << "() {\n";
        for(BasicBlock* bb : _basicBlockList)
            bb->Print(stream, indent + 2);
        stream << std::string(indent, ' ') << "}";
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
}