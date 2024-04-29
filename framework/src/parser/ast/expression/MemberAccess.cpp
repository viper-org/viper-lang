// Copyright 2024 solar-mist


#include "parser/ast/expression/MemberAccess.h"

#include "type/StructType.h"

#include <vipir/IR/Instruction/GEPInst.h>
#include <vipir/IR/Instruction/LoadInst.h>

#include <vipir/Module.h>

namespace parser
{
    MemberAccess::MemberAccess(ASTNodePtr struc, std::string field)
        : mStruct(std::move(struc))
        , mField(std::move(field))
    {
        StructType* structType = static_cast<StructType*>(mStruct->getType());
        
    }

    vipir::Value* MemberAccess::emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope)
    {
        vipir::Value* structValue = mStruct->emit(builder, module, scope);
        vipir::Value* pointerOperand = vipir::getPointerOperand(structValue);
        
        vipir::Instruction* instruction = static_cast<vipir::Instruction*>(structValue);
        instruction->eraseFromParent();
        
        StructType* structType = static_cast<StructType*>(mStruct->getType());

        vipir::Value* gep = builder.CreateStructGEP(pointerOperand, structType->getFieldOffset(mField));

        return builder.CreateLoad(gep);
    }
}