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
#include <ssa/value/instruction/load.hh>
#include <ssa/value/instruction/binOp.hh>
#include <ssa/value/basicBlock.hh>
#include <ssa/value/instruction/call.hh>

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

        Value* CreateAdd(Value* lhs, Value* rhs);
        Value* CreateSub(Value* lhs, Value* rhs);
        Value* CreateMul(Value* lhs, Value* rhs);
        Value* CreateDiv(Value* lhs, Value* rhs);

        Value* CreateCmp(Instruction::InstType op, Value* lhs, Value* rhs);

        CallInst* CreateCall(Function* callee, const std::vector<Value*>& args, bool isStatement, const std::string& name = "");

        AllocaInst* CreateAlloca(std::shared_ptr<Type> allocatedType, const std::string& name = "", bool isArg = false);
        StoreInst* CreateStore(Value* ptr, Value* value);
        LoadInst* CreateLoad(Value* ptr, const std::string& name = "");

        Module& GetModule() const;
    private:
        Value* CreateBinOp(Instruction::InstType op, Value* lhs, Value* rhs);

        Module& _module;
        BasicBlock* _insertPoint;
    };
}

#endif