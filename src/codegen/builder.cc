#include <codegen/builder.hh>

namespace Codegen
{
    Builder::Builder(Module& module)
        :_module(module)
    {
    }

    void Builder::SetInsertPoint(BasicBlock* insertPoint)
    {
        _insertPoint = insertPoint;
    }

    Value* Builder::CreateIntLiteral(long long value, Type* type)
    {
        IntegerLiteral* literal = new IntegerLiteral(value, type, _module);
        
        return literal;
    }


    Value* Builder::CreateRet(Value* value)
    {
        RetInst* ret = new RetInst(value, _module);

        _insertPoint->GetInstrList().push_back(ret);

        return ret;
    }


    Value* Builder::CreateBinOp(Value* left, Instruction::Instructions op, Value* right)
    {
        return BinaryOperator::Create(left, op, right, _module);
    }

    Value* Builder::CreateAdd(Value *left, Value *right)
    {
        return CreateBinOp(left, Instruction::Add, right);
    }

    Value* Builder::CreateSub(Value *left, Value *right)
    {
        return CreateBinOp(left, Instruction::Sub, right);
    }

    Value* Builder::CreateMul(Value *left, Value *right)
    {
        return CreateBinOp(left, Instruction::Mul, right);
    }


    CallInst* Builder::CreateCall(Function* callee, std::vector<Value*> args, bool isStatement)
    {
        CallInst* call = new CallInst(callee, args, _module);

        if(isStatement)
            _insertPoint->GetInstrList().push_back(call);

        return call;
    }


    AllocaInst* Builder::CreateAlloca(Type* type)
    {
        AllocaInst* alloca = new AllocaInst(type, 0, _module);

        _insertPoint->GetParent()->AddAlloca(alloca);

        return alloca;
    }

    StoreInst* Builder::CreateStore(Value* value, Value* ptr, bool isStatement)
    {
        StoreInst* store = new StoreInst(value, ptr, _module);

        if(isStatement)
            _insertPoint->GetInstrList().push_back(store);

        return store;
    }

    LoadInst* Builder::CreateLoad(Value* ptr)
    {
        LoadInst* load = new LoadInst(ptr, _module);

        return load;
    }

    SExtInst* Builder::CreateSExt(Value* value, Type* dstType)
    {
        SExtInst* sext = new SExtInst(value, dstType, _module);

        return sext;
    }
}