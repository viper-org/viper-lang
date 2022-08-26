#include <codegen/builder.hh>

namespace Codegen
{
    Builder::Builder(const Module& module)
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
}