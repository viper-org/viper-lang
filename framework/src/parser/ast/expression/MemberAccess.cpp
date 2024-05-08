// Copyright 2024 solar-mist


#include "parser/ast/expression/MemberAccess.h"

#include "type/StructType.h"
#include "type/PointerType.h"

#include <vipir/IR/Instruction/GEPInst.h>
#include <vipir/IR/Instruction/LoadInst.h>

#include <vipir/Module.h>

namespace parser
{
    MemberAccess::MemberAccess(ASTNodePtr struc, std::string field, bool pointer)
        : mStruct(std::move(struc))
        , mField(std::move(field))
        , mPointer(pointer)
    {

        StructType* structType;
        if (mPointer)
        {
            structType = static_cast<StructType*>(static_cast<PointerType*>(mStruct->getType())->getBaseType());
        }
        else
        {
            structType = static_cast<StructType*>(mStruct->getType());
        }

        mType = structType->getField(mField).type;
    }

    vipir::Value* MemberAccess::emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope)
    {
        vipir::Value* struc;
        if (mPointer)
        {
            struc = mStruct->emit(builder, module, scope);
        }
        else
        {
            vipir::Value* structValue = mStruct->emit(builder, module, scope);
            struc = vipir::getPointerOperand(structValue);

            vipir::Instruction* instruction = static_cast<vipir::Instruction*>(structValue);
            instruction->eraseFromParent();
        }

        StructType* structType;
        if (mPointer)
        {
            structType = static_cast<StructType*>(static_cast<PointerType*>(mStruct->getType())->getBaseType());
        }
        else
        {
            structType = static_cast<StructType*>(mStruct->getType());
        }

        vipir::Value* gep = builder.CreateStructGEP(struc, structType->getFieldOffset(mField));

        return builder.CreateLoad(gep);
    }
}
