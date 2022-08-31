#include <codegen/value/instruction/sext.hh>
#include <codegen/value/instruction/load.hh>
#include <iostream>

namespace Codegen
{
    SExtInst::SExtInst(Value* value, Type* type, const Module& module)
        :Instruction(module), _value(value), _dstType(type)
    {
    }

    const std::pair<std::string, Register*> SExtInst::Generate(Register*)
    {
        std::string result;
        Register* reg;
        if(LoadInst* load = dynamic_cast<LoadInst*>(_value))
        {
            reg = Register::GetRegister();
            result += "\n\tmovs" + _value->GetType()->GetSuffix() + _dstType->GetSuffix();
            result += " " + load->GetPtr()->Generate().first + ", " + reg->GetID(_dstType->GetSize());
        }
        else
        {
            std::pair<std::string, Register*> valueCodegen = _value->Generate();
            result = valueCodegen.first;
            reg = Register::GetRegister();
            result += "\n\tmovs" + _value->GetType()->GetSuffix() + _dstType->GetSuffix();
            result += " " + valueCodegen.second->GetID(_dstType->GetSize());
        }
        
        return std::make_pair(result, reg);
    }
}