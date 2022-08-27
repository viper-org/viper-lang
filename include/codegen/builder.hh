#ifndef VIPER_CODEGEN_BUILDER_HH
#define VIPER_CODEGEN_BUILDER_HH
#include <codegen/module.hh>
#include <codegen/value/value.hh>
#include <codegen/value/global/global.hh>
#include <codegen/value/global/function.hh>
#include <codegen/value/constant/intLiteral.hh>
#include <codegen/value/instruction/instruction.hh>
#include <codegen/value/instruction/ret.hh>
#include <codegen/value/basicBlock.hh>

namespace Codegen
{
    class Builder
    {
    public:
        Builder(Module& module);

        void SetInsertPoint(BasicBlock* insertPoint);

        Value* CreateIntLiteral(long long value);

        Value* CreateRet(Value* value);
    
    private:
        Module& _module;
        BasicBlock* _insertPoint;
    };
}

#endif