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
}