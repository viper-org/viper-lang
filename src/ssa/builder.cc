#include <ssa/builder.hh>

namespace SSA
{
    Builder::Builder(Module& module)
        :_module(module), _insertPoint(nullptr)
    {
    }

    void Builder::SetInsertPoint(BasicBlock* insertPoint)
    {
        _insertPoint = insertPoint;
    }

    BasicBlock* Builder::GetInsertPoint() const
    {
        return _insertPoint;
    }


    Value* Builder::CreateConstantInt(long long value)
    {
        IntegerLiteral* literal = new IntegerLiteral(_module, value);

        return literal;
    }

    Value* Builder::CreateRet(Value* value)
    {
        RetInst* ret = new RetInst(_module, value);

        _insertPoint->GetInstList().push_back(ret);

        return ret;
    }

    AllocaInst* Builder::CreateAlloca(const std::string& name)
    {
        AllocaInst* alloca = new AllocaInst(_module, name);

        _insertPoint->GetParent()->GetAllocaList().push_back(alloca);

        return alloca;
    }

    StoreInst* Builder::CreateStore(Value* ptr, Value* value)
    {
        StoreInst* store = new StoreInst(_module, ptr, value);

        _insertPoint->GetInstList().push_back(store);

        return store;
    }

    LoadInst* Builder::CreateLoad(Value* ptr, const std::string& name)
    {
        LoadInst* load = new LoadInst(_module, ptr, name);

        return load;
    }

    Module& Builder::GetModule() const
    {
        return _module;
    }
}