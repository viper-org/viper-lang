#ifndef VIPER_CODEGEN_CALL_HH
#define VIPER_CODEGEN_CALL_HH
#include <codegen/value/instruction/instruction.hh>
#include <codegen/value/global/function.hh>

namespace Codegen
{
    class CallInst : public Instruction // TODO: Add args
    {
    friend class Builder;
    public:
        const std::pair<std::string, Register*> Generate(Register* reg = nullptr) override;
    protected:
        CallInst(Function* callee, std::vector<Value*> args, const Module& module);
    private:
        Function* _callee;
        std::vector<Value*> _args;
    };
}

#endif