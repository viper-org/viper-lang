#include <codegen/builder.hh>

namespace Codegen
{
    Builder::Builder(Module& module)
        :_module(module)
    {
    }

    void Builder::SetInsertFunction(Function* insertFunction)
    {
        _insertFunction = insertFunction;
    }

    Value* Builder::CreateIntLiteral(long long value)
    {
        IntegerLiteral* literal = new IntegerLiteral(value, _module);

        return literal;
    }

    Value* Builder::CreateRet(Value* value)
    {
        RetVal* ret = new RetVal(value, _module);

        _module.GetFunctionBodies()[_insertFunction].push_back(ret);

        return ret;
    }
}