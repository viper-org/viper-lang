#ifndef VIPER_SSA_BUILDER_HH
#define VIPER_SSA_BUILDER_HH
#include <ssa/module.hh>
#include <ssa/value/value.hh>
#include <ssa/value/global/function.hh>
#include <ssa/value/constant/integer.hh>
#include <ssa/value/instruction/instruction.hh>
#include <ssa/value/instruction/return.hh>
#include <ssa/value/instruction/alloca.hh>
#include <ssa/value/instruction/store.hh>
#include <ssa/value/basicBlock.hh>

namespace SSA
{
    class Builder
    {
    public:
        Builder(Module& module);

        void SetInsertPoint(BasicBlock* insertPoint);
        BasicBlock* GetInsertPoint() const;

        Value* CreateRet(Value* value);

        Value* CreateConstantInt(long long value);

        AllocaInst* CreateAlloca(const std::string& name = "");
        StoreInst* CreateStore(Value* ptr, Value* value);

        Module& GetModule() const;
    private:
        Module& _module;
        BasicBlock* _insertPoint;
    };
}

#endif