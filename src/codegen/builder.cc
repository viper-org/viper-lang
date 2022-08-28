#include <codegen/builder.hh>

namespace Codegen
{
    Builder::Builder(Module& module)
        :_module(module), _allocaOffset(-8)
    {
    }

    void Builder::SetInsertPoint(BasicBlock* insertPoint)
    {
        _insertPoint = insertPoint;
    }

    Value* Builder::CreateIntLiteral(long long value)
    {
        IntegerLiteral* literal = new IntegerLiteral(value, _module);
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


    CallInst* Builder::CreateCall(Function* callee, std::vector<Value*> args)
    {
        CallInst* call = new CallInst(callee, args, _module);

        return call;
    }


    AllocaInst* Builder::CreateAlloca()
    {
        AllocaInst* alloca = new AllocaInst(_allocaOffset, _module);

        _allocaOffset -= 8;

        return alloca;
    }

    StoreInst* Builder::CreateStore(Value* value, Value* ptr)
    {
        StoreInst* store = new StoreInst(value, ptr, _module);

        _insertPoint->GetInstrList().push_back(store);

        return store;
    }

    LoadInst* Builder::CreateLoad(Value* ptr)
    {
        LoadInst* load = new LoadInst(ptr, _module);

        return load;
    }
}