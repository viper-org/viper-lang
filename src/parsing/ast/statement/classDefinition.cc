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
            std::vector<llvm::Type*> paramTypes;
            std::vector<std::shared_ptr<Type>> argTypes;
            paramTypes.push_back(llvm::PointerType::get(types.at(_name)->GetLLVMType(), 0));
            argTypes.push_back(types.at(_name));
            for(std::pair<std::shared_ptr<Type>, std::string> param : method.params)
            {
                paramTypes.push_back(param.first->GetLLVMType());
                argTypes.push_back(param.first);
            }

            std::string mangledName = MangleFunction({_name, method.name}, argTypes, method.returnType);

            llvm::FunctionType* funcTy = llvm::FunctionType::get(method.returnType->GetLLVMType(), paramTypes, false);
            llvm::Function* func = llvm::Function::Create(funcTy, llvm::GlobalValue::ExternalLinkage, mangledName, mod);

            func->args().begin()->setName("this");
            for(unsigned int i = 1; i < func->arg_size(); i++)
                (func->args().begin()+i)->setName(method.params[i-1].second);

            llvm::BasicBlock* bb = llvm::BasicBlock::Create(ctx, mangledName, func);
            builder.SetInsertPoint(bb);

            for(llvm::Argument& param : func->args())
            {
                llvm::AllocaInst* alloca = builder.CreateAlloca(param.getType(), nullptr, param.getName());
                builder.CreateStore(&param, alloca);
                method.scope->GetNamedValues()[param.getName().str()] = alloca;
            }

            method.body->Emit(ctx, mod, builder, method.scope);

            llvm::EliminateUnreachableBlocks(*func);

            if(func->getBasicBlockList().back().getInstList().size() == 0 || !func->getBasicBlockList().back().getInstList().back().isTerminator())
                builder.CreateRet(llvm::Constant::getNullValue(method.returnType->GetLLVMType()));
        }

        return nullptr;
    }
}