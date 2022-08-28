#include <codegen/value/instruction/call.hh>

namespace Codegen
{
    CallInst::CallInst(Function* callee, std::vector<Value*> args, const Module& module)
        :Instruction(module), _callee(callee), _args(args)
    {
    }

    const std::pair<std::string, Register*> CallInst::Generate(Register* reg)
    {
        const std::string argRegs[] = { "%rdi", "%rsi", "%rdx", "%rcx", "%r8", "%r9" };
        std::string result;

        unsigned int index = 0;
        for(Value* arg : _args)
        {
            result += arg->Generate(Register::FindRegister(argRegs[index++])).first;
            delete arg;
        }

        result += "\n\tcall " + _callee->GetName();
        if(_callee->IsDecl())
            result += "@PLT";
        if(!reg)
            reg = Register::GetRegister();
        if(reg->GetID() != "%rax")
            result += "\n\tmovq %rax, " + reg->GetID();
        
        return std::make_pair(result, reg);
    }
}