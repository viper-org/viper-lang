#include <llvm/Transforms/Utils/BasicBlockUtils.h>
#include <parsing/ast/statement/classDefinition.hh>

namespace Parsing
{
    ClassDefinition::ClassDefinition(const std::string& name, const std::vector<ClassField>& fields, std::vector<ClassMethod> methods)
        :ASTNode(ASTNodeType::ClassDefinition), _name(name), _fields(fields), _methods(std::move(methods))
    {
    }

    void ClassDefinition::Print(std::ostream&, int) const
    {
        // todo: implement
    }

    llvm::Value* ClassDefinition::Emit(llvm::LLVMContext& ctx, llvm::Module& mod, llvm::IRBuilder<>& builder, std::shared_ptr<Environment>)
    {
        for(ClassMethod& method : _methods)
        {
            bool isCtor = false;
            if(method.returnType == nullptr)
                isCtor = true;
            std::vector<llvm::Type*> paramTypes;
            std::vector<std::shared_ptr<Type>> argTypes;
            std::shared_ptr<Type> returnType = (isCtor) ? types.at(_name) : method.returnType;
            if(!isCtor)
            {
                paramTypes.push_back(llvm::PointerType::get(types.at(_name)->GetLLVMType(), 0));
                argTypes.push_back(types.at(_name));
            }
            for(std::pair<std::shared_ptr<Type>, std::string> param : method.params)
            {
                paramTypes.push_back(param.first->GetLLVMType());
                argTypes.push_back(param.first);
            }
            
            std::string mangledName = MangleFunction({_name, method.name}, argTypes, returnType);

            llvm::FunctionType* funcTy = llvm::FunctionType::get(returnType->GetLLVMType(), paramTypes, false);
            llvm::Function* func = llvm::Function::Create(funcTy, llvm::GlobalValue::ExternalLinkage, mangledName, mod);

            if(!isCtor)
                func->args().begin()->setName("this");
            unsigned int i = isCtor ? 0 : 1;
            for(; i < func->arg_size(); i++)
            {
                if(isCtor)
                    (func->args().begin()+i)->setName(method.params[i].second);
                else
                    (func->args().begin()+i)->setName(method.params[i-1].second);
            }

            llvm::BasicBlock* bb = llvm::BasicBlock::Create(ctx, mangledName, func);
            builder.SetInsertPoint(bb);

            for(llvm::Argument& param : func->args())
            {
                llvm::AllocaInst* alloca = builder.CreateAlloca(param.getType(), nullptr, param.getName());
                builder.CreateStore(&param, alloca);
                method.scope->GetNamedValues()[param.getName().str()] = alloca;
            }

            llvm::AllocaInst* self;
            if(isCtor)
            {
                self = builder.CreateAlloca(types.at(_name)->GetLLVMType(), nullptr);
                method.scope->GetNamedValues()["this"] = self;
            }

            method.body->Emit(ctx, mod, builder, method.scope);

            llvm::EliminateUnreachableBlocks(*func);

            if(isCtor)
                builder.CreateRet(builder.CreateLoad(self->getAllocatedType(), self));
            else
                if(func->getBasicBlockList().back().getInstList().size() == 0 || !func->getBasicBlockList().back().getInstList().back().isTerminator())
                    builder.CreateRet(llvm::Constant::getNullValue(method.returnType->GetLLVMType()));
        }

        return nullptr;
    }
}