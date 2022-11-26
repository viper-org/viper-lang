#include <parsing/ast/statement/variableDeclaration.hh>
#include <environment.hh>
#include <llvm/IR/BasicBlock.h>
#include <llvm/Transforms/Utils/BasicBlockUtils.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>

namespace Parsing
{
    VariableDeclaration::VariableDeclaration(const std::string& name, std::unique_ptr<ASTNode> initVal, std::shared_ptr<Environment> scope, std::shared_ptr<Type> type, std::vector<std::pair<std::shared_ptr<Type>, std::string>> params)
        :ASTNode(ASTNodeType::VariableDeclaration), _name(name), _initVal(std::move(initVal)), _scope(scope), _params(params)
    {
        _nodeType = (scope != nullptr ? ASTNodeType::Function : ASTNodeType::VariableDeclaration);
        _type = type;
    }

    void VariableDeclaration::Print(std::ostream& stream, int indent) const
    {
        stream << std::string(indent, ' ') << (_scope != nullptr ? "<Function>:\n" : "<Variable-Declaration>:\n");
        stream << std::string(indent, ' ') << "Name: " << _name;
        if(_initVal)
        {
            stream << "\n" << std::string(indent, ' ') << "Value: \n";
            _initVal->Print(stream, indent + 2);
        }
    }

    llvm::Value* VariableDeclaration::Emit(llvm::LLVMContext& ctx, llvm::Module& mod, llvm::IRBuilder<>& builder, std::shared_ptr<Environment> scope)
    {
        if(_scope != nullptr)
        {
            std::vector<llvm::Type*> paramTypes;
            std::vector<std::shared_ptr<Type>> argTypes;
            for(std::pair<std::shared_ptr<Type>, std::string> param : _params)
            {
                paramTypes.push_back(param.first->GetLLVMType());
                argTypes.push_back(param.first);
            }

            std::string mangledName = MangleFunction(_name, argTypes, _type);
            
            llvm::FunctionType* funcTy = llvm::FunctionType::get(_type->GetLLVMType(), paramTypes, false);
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
                builder.CreateRet(llvm::Constant::getNullValue(_type->GetLLVMType()));

            return func;
        }
        llvm::AllocaInst* alloca = builder.CreateAlloca(_type->GetLLVMType(), nullptr, _name);
        if(_initVal)
        {
            llvm::Value* initVal = _initVal->Emit(ctx, mod, builder, scope);
            initVal = Type::Convert(initVal, _type->GetLLVMType(), builder);
            builder.CreateStore(initVal, alloca);
        }
        scope->GetNamedValues()[_name] = alloca;
        return nullptr;
    }
}