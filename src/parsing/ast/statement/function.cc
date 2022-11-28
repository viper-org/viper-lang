#include <parsing/ast/statement/function.hh>
#include <environment.hh>
#include <llvm/IR/BasicBlock.h>
#include <llvm/Transforms/Utils/BasicBlockUtils.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>

namespace Parsing
{
    Function::Function(const std::string& name, std::unique_ptr<ASTNode> initVal, std::shared_ptr<Environment> scope, std::shared_ptr<Type> returnType, std::vector<std::pair<std::shared_ptr<Type>, std::string>> params)
        :ASTNode(ASTNodeType::Function), _name(name), _initVal(std::move(initVal)), _scope(scope), _returnType(returnType), _params(params)
    {
    }

    void Function::Print(std::ostream& stream, int indent) const
    {
        stream << std::string(indent, ' ') << "<Function>:\n";
        stream << std::string(indent, ' ') << "Name: " << _name;
        stream << "\n" << std::string(indent, ' ') << "Value: \n";
        _initVal->Print(stream, indent + 2);
    }

    llvm::Value* Function::Emit(llvm::LLVMContext& ctx, llvm::Module& mod, llvm::IRBuilder<>& builder, std::shared_ptr<Environment>)
    {
        std::vector<llvm::Type*> paramTypes;
        std::vector<std::shared_ptr<Type>> argTypes;
        for(std::pair<std::shared_ptr<Type>, std::string> param : _params)
        {
            paramTypes.push_back(param.first->GetLLVMType());
            argTypes.push_back(param.first);
        }

        std::string mangledName = MangleFunction({_name}, argTypes, _returnType);
        
        llvm::FunctionType* funcTy = llvm::FunctionType::get(_returnType->GetLLVMType(), paramTypes, false);
        llvm::Function* func = llvm::Function::Create(funcTy, llvm::GlobalValue::ExternalLinkage, mangledName, mod);

        unsigned int i = 0;
        for(llvm::Argument& param : func->args())
            param.setName(_params[i++].second);

        llvm::BasicBlock* bb = llvm::BasicBlock::Create(ctx, mangledName, func);
        builder.SetInsertPoint(bb);

        for(llvm::Argument& param : func->args())
        {
            llvm::AllocaInst* alloca = builder.CreateAlloca(param.getType(), nullptr, param.getName());
            builder.CreateStore(&param, alloca);
            _scope->GetNamedValues()[param.getName().str()] = alloca;
        }

        _initVal->Emit(ctx, mod, builder, _scope);

        llvm::EliminateUnreachableBlocks(*func);

        if(func->getBasicBlockList().back().getInstList().size() == 0 || !func->getBasicBlockList().back().getInstList().back().isTerminator())
            builder.CreateRet(llvm::Constant::getNullValue(_returnType->GetLLVMType()));

        return func;
    }
}