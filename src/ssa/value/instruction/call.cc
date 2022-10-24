#include <ssa/value/instruction/call.hh>
#include <ssa/value/global/function.hh>
#include <environment.hh>
#include <array>

namespace SSA
{
    CallInst::CallInst(Module& module, Function* callee, const std::vector<Value*>& args, const std::string& name)
        :Instruction(module), _name(new TempValue(module, name)), _callee(callee), _args(args)
    {
        _instType = Instruction::Call;
    }

    std::string CallInst::GetID() const
    {
        return _name->GetID();
    }

    void CallInst::Print(std::ostream& stream, int indent) const
    {
        _name->Print(stream, indent);
        stream << "call int32 " << _callee->GetID() << '\n';
    }

    Codegen::Value* CallInst::Emit(Codegen::Assembly& assembly)
    {
        const std::array argRegs = {
            Codegen::Register::GetRegister("rdi"),
            Codegen::Register::GetRegister("rsi"),
            Codegen::Register::GetRegister("rdx"),
            Codegen::Register::GetRegister("rcx"),
        };
        std::vector<Codegen::Value*> argValues;
        for(Value* arg : _args)
            argValues.push_back(arg->Emit(assembly));

        unsigned int index = 0;
        for(Codegen::Value* argValue : argValues)
        {
            assembly.CreateMov(argRegs[index++], argValue);
            argValue->Dispose();
        }

        assembly.CreateCall(_callee->GetName());
        
        return Codegen::Register::GetRegister("rax");
    }

    void CallInst::Dispose()
    {
        _name->Dispose();
        for(Value* arg : _args)
            arg->Dispose();
        delete this;
    }
}