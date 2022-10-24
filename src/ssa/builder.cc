#include "ssa/value/instruction/call.hh"
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

    Value* Builder::CreateAdd(Value* lhs, Value* rhs)
    {
        return CreateBinOp(Instruction::Add, lhs, rhs);
    }

    Value* Builder::CreateSub(Value* lhs, Value* rhs)
    {
        return CreateBinOp(Instruction::Sub, lhs, rhs);
    }

    Value* Builder::CreateMul(Value* lhs, Value* rhs)
    {
        return CreateBinOp(Instruction::Mul, lhs, rhs);
    }

    Value* Builder::CreateDiv(Value* lhs, Value* rhs)
    {
        return CreateBinOp(Instruction::Div, lhs, rhs);
    }

    CallInst* Builder::CreateCall(Function* callee, const std::vector<Value*>& args, bool isStatement, const std::string& name)
    {
        CallInst* call = new CallInst(_module, callee, args, name);

        if(isStatement)
            _insertPoint->GetInstList().push_back(call);

        return call;
    }

    AllocaInst* Builder::CreateAlloca(std::shared_ptr<Type> allocatedType, const std::string& name, bool isArg)
    {
        AllocaInst* alloca = new AllocaInst(_module, allocatedType, name);

        if(!isArg)
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

    Value* Builder::CreateBinOp(Instruction::InstType op, Value* lhs, Value* rhs)
    {
        BinOp* binop = new BinOp(_module, op, lhs, rhs);

        return binop;
    }
}