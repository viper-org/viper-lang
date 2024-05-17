// Copyright 2024 solar-mist


#include "parser/ast/expression/MemberAccess.h"

#include "type/StructType.h"
#include "type/PointerType.h"

#include <vipir/IR/Instruction/GEPInst.h>
#include <vipir/IR/Instruction/LoadInst.h>
#include <vipir/IR/Instruction/PtrCastInst.h>

#include <vipir/Type/PointerType.h>

#include <vipir/Module.h>

#include <iostream>

namespace parser
{
    MemberAccess::MemberAccess(ASTNodePtr struc, std::string field, bool pointer, lexing::Token fieldToken)
        : mStruct(std::move(struc))
        , mField(field)
        , mPointer(pointer)
        , mFieldToken(std::move(fieldToken))
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

        auto structField = structType->getField(mField);
        if (structField)
            mType = structField->type;
    }

    vipir::Value* MemberAccess::emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope, diagnostic::Diagnostics& diag)
    {
        vipir::Value* struc;
        if (mPointer)
        {
            struc = mStruct->emit(builder, module, scope, diag);
        }
        else
        {
            vipir::Value* structValue = mStruct->emit(builder, module, scope, diag);
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

        if (!structType->hasField(mField))
        {
            diag.compilerError(mFieldToken.getStart(), mFieldToken.getEnd(), std::format("'{}struct {}{}' has no member named '{}{}{}'",
                fmt::bold, structType->getName(), fmt::defaults, fmt::bold, mField, fmt::defaults));
        }
        if (structType->getField(mField)->priv && scope->findOwner() != structType)
        {
            diag.compilerError(mFieldToken.getStart(), mFieldToken.getEnd(), std::format("'{}{}{}' is a private member of '{}struct {}{}'",
                fmt::bold, mField, fmt::defaults, fmt::bold, structType->getName(), fmt::defaults));
        }

        vipir::Value* gep = builder.CreateStructGEP(struc, structType->getFieldOffset(mField));

        // struct types with a pointer to themselves cannot be emitted normally
        if (structType->getField(mField)->type->isPointerType())
        {
            if (static_cast<PointerType*>(structType->getField(mField)->type)->getBaseType() == structType)
            {
                vipir::Type* type = vipir::PointerType::GetPointerType(vipir::PointerType::GetPointerType(structType->getVipirType()));
                gep = builder.CreatePtrCast(gep, type);
            }
        }

        return builder.CreateLoad(gep);
    }
}
