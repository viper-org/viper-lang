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
        Return* ret = new Return(_module, value);

        if(_insertPoint)
            _insertPoint->GetInstList().push_back(ret);

        return ret;
    }

    Module& Builder::GetModule() const
    {
        return _module;
    }
}