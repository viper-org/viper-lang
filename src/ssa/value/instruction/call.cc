#include <ssa/value/instruction/call.hh>
#include <ssa/value/global/function.hh>
#include <environment.hh>

namespace SSA
{
    CallInst::CallInst(Module& module, Function* callee, const std::string& name)
        :Instruction(module), _name(new TempValue(module, name)), _callee(callee)
    {
        _instType = Instruction::Load;
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
        assembly.CreateCall(_callee->GetName());
        
        return Codegen::Register::GetRegister("rax");
    }

    void CallInst::Dispose()
    {
        _name->Dispose();
        delete this;
    }
}