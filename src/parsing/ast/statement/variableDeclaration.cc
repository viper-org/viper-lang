#include <parsing/ast/statement/variableDeclaration.hh>
#include <diagnostics.hh>
#include <llvm/IR/Constant.h>
#include <llvm/IR/Constants.h>
#include <environment.hh>
#include <llvm/IR/BasicBlock.h>
#include <llvm/Transforms/Utils/BasicBlockUtils.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>

namespace Parsing
{
    VariableDeclaration::VariableDeclaration(const std::string& name, std::unique_ptr<ASTNode> initVal, std::shared_ptr<Type> type, bool isGlobalVar)
        :ASTNode(ASTNodeType::VariableDeclaration), _name(name), _initVal(std::move(initVal)), _isGlobalVar(isGlobalVar)
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

    bool VariableDeclaration::IsGlobal() const
    {
        return _isGlobalVar;
    }

    llvm::Value* VariableDeclaration::Emit(llvm::LLVMContext& ctx, llvm::Module& mod, llvm::IRBuilder<>& builder, std::shared_ptr<Environment> scope)
    {
        if(_isGlobalVar)
        {
            mod.getOrInsertGlobal(_name, _type->GetLLVMType());
            llvm::GlobalVariable* global = mod.getGlobalVariable(_name);

            llvm::Constant* initializer = llvm::Constant::getNullValue(_type->GetLLVMType());

            if(_initVal)
            {
                llvm::Value* initValue = _initVal->Emit(ctx, mod, builder, scope);
                if(llvm::Constant* constant = static_cast<llvm::Constant*>(initValue))
                    initializer = constant;
                else
                    Diagnostics::Error("viper", "Initializer element is not constant");
            }

            global->setInitializer(initializer);
            global->setLinkage(llvm::GlobalValue::InternalLinkage);

            return nullptr;
        }
        llvm::AllocaInst* alloca = builder.CreateAlloca(_type->GetLLVMType(), nullptr, _name);
        if(_initVal)
        {
            llvm::Value* initVal = _initVal->Emit(ctx, mod, builder, scope);
            if(alloca->getType()->isPointerTy() && alloca->getType()->getNonOpaquePointerElementType()->isArrayTy())
            {
                unsigned int elements = alloca->getAllocatedType()->getArrayNumElements();
                llvm::MaybeAlign align(elements < 16 ? 1 : 16);
                builder.CreateMemCpy(alloca, align, initVal, align, elements);
            }
            else
                builder.CreateStore(initVal, alloca);
        }
        scope->GetNamedValues()[_name] = alloca;
        return nullptr;
    }
}