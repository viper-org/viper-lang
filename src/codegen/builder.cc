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

    Value* Builder::CreateIntLiteral(long long value)
    {
        IntegerLiteral* literal = new IntegerLiteral(value, _module);
        return literal;
    }


    Value* Builder::CreateRet(Value* value)
    {
        RetVal* ret = new RetVal(value, _module);

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
}