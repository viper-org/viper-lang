#include <iostream>
#include <llvm/IR/Constant.h>
#include <llvm/IR/Constants.h>
#include <parsing/ast/statement/variableDeclaration.hh>
#include <environment.hh>
#include <llvm/IR/BasicBlock.h>
#include <llvm/Transforms/Utils/BasicBlockUtils.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>

namespace Parsing
{
    VariableDeclaration::VariableDeclaration(const std::string& name, std::unique_ptr<ASTNode> initVal, std::shared_ptr<Type> type)
        :ASTNode(ASTNodeType::VariableDeclaration), _name(name), _initVal(std::move(initVal))
    {
        _type = type;
    }

    void VariableDeclaration::Print(std::ostream& stream, int indent) const
    {
        stream << std::string(indent, ' ') << "<Variable-Declaration>:\n";
        stream << std::string(indent, ' ') << "Name: " << _name;
        if(_initVal)
        {
            stream << "\n" << std::string(indent, ' ') << "Value: \n";
            _initVal->Print(stream, indent + 2);
        }
    }

    llvm::Value* VariableDeclaration::Emit(llvm::LLVMContext& ctx, llvm::Module& mod, llvm::IRBuilder<>& builder, std::shared_ptr<Environment> scope)
    {
        llvm::AllocaInst* alloca = builder.CreateAlloca(_type->GetLLVMType(), nullptr, _name);
        if(_initVal)
        {
            llvm::Value* initVal = _initVal->Emit(ctx, mod, builder, scope);
            if(alloca->getType()->isPointerTy())
            {
                if(alloca->getType()->getNonOpaquePointerElementType()->isArrayTy())
                {
                    unsigned int elements = alloca->getAllocatedType()->getArrayNumElements();
                    llvm::MaybeAlign align(elements < 16 ? 1 : 16);
                    builder.CreateMemCpy(alloca, align, initVal, align, elements);
                }
            }
            else
                builder.CreateStore(initVal, alloca);
        }
        scope->GetNamedValues()[_name] = alloca;
        return nullptr;
    }
}