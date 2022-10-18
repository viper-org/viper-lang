#include <ssa/value/global/function.hh>
#include <algorithm>

namespace SSA
{
    Function* Function::Create(Module& module, const std::string& name)
    {
        Function* func = new Function(module, name);

        module.GetGlobals().push_back(func);

        return func;
    }

    Function::Function(Module& module, const std::string& name)
        :Value(module), _name(name), _totalAllocaOffset(0)
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
        for(AllocaInst* alloca : _allocaList)
            alloca->Print(stream, indent + 2);
        for(BasicBlock* bb : _basicBlockList)
            bb->Print(stream, indent + 2);
        stream << std::string(indent, ' ') << "}";
    }

    std::string Function::GetID() const
    {
        return "%" + _name;
    }

    std::string_view Function::GetName() const
    {
        return _name;
    }

    Codegen::Value* Function::Emit(Codegen::Assembly& assembly)
    {
        SortAllocas();
        assembly.CreateGlobal(_name);
        assembly.CreateLabel(_name);

        if(_totalAllocaOffset)
        {
            Codegen::Register* rbp = Codegen::Register::GetRegister("rbp");
            Codegen::Register* rsp = Codegen::Register::GetRegister("rsp");
            Codegen::ImmediateValue* rspOffset = new Codegen::ImmediateValue(_totalAllocaOffset, types.at("int64"));

            assembly.CreatePush(rbp);
            assembly.CreateMov(rbp, rsp);
            assembly.CreateSub(rsp, rspOffset);

            rspOffset->Dispose();
        }

        for(BasicBlock* bb : _basicBlockList)
            bb->Emit(assembly);

        assembly.CreateLabel(".ret");
        if(_totalAllocaOffset)
            assembly.CreateLeave();
        assembly.CreateRet();

        return nullptr;
    }

    void Function::Dispose()
    {
        for(BasicBlock* basicBlock : _basicBlockList)
            basicBlock->Dispose();
        for(AllocaInst* alloca : _allocaList)
            alloca->Dispose();

        delete this;
    }

    void Function::SortAllocas()
    {
        std::sort(_allocaList.begin(), _allocaList.end(), [](AllocaInst* lhs, AllocaInst* rhs) {
            return lhs->GetAllocatedType()->GetScalarSize() > rhs->GetAllocatedType()->GetScalarSize();
        });
        int offset = 0;
        for(AllocaInst* alloca : _allocaList)
        {
            offset += alloca->GetAllocatedType()->GetScalarSize() / 8;
            alloca->_offset = offset;
        }
        _totalAllocaOffset = offset + 15 & ~15;
    }
}