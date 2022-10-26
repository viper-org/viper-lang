#include <ssa/value/instruction/branch.hh>

namespace SSA
{
    BranchInst::BranchInst(Module& module, Value* cond, BasicBlock* trueBranch, BasicBlock* falseBranch)
        :Instruction(module), _cond(cond), _true(trueBranch), _false(falseBranch)
    {
    }

    BranchInst::BranchInst(Module& module, BasicBlock* branch)
        :Instruction(module), _cond(nullptr), _true(branch)
    {
    }

    void BranchInst::Print(std::ostream& stream, int indent) const
    {
        stream << std::string(indent, ' ') << "jmp i1 " << _cond->GetID();
        stream << ", " << _true->GetID() << ", " << _false->GetID();
    }

    std::string BranchInst::GetID() const
    {
        return "";
    }

    Codegen::Value* BranchInst::Emit(Codegen::Assembly& assembly)
    {
        if(!_cond)
        {
            assembly.CreateJmp(_true->GetName());
            return nullptr;
        }
        Codegen::Value* condValue = _cond->Emit(assembly);
        if(!condValue->IsCompare())
        {
            Codegen::ImmediateValue* zero = new Codegen::ImmediateValue(0, types.at("int64"));
            assembly.CreateCmp(condValue, zero);
            zero->Dispose();
            Codegen::Compare* cmp = new Codegen::Compare(Codegen::CompareOperator::NE);
            condValue->Dispose();
            condValue = cmp;
        }
        assembly.CreateCndJmp(_true->GetName(), condValue);
        condValue->Dispose();
        return nullptr;
    }

    void BranchInst::Dispose()
    {
        if(_cond)
            _cond->Dispose();
        delete this;
    }
}