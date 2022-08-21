#include <parsing/AST/topLevel/function.hxx>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/BasicBlock.h>

namespace Viper
{
    namespace Parsing
    {
        ASTFunction::ASTFunction(std::string name, std::shared_ptr<Type> type, std::vector<std::pair<std::shared_ptr<Type>, std::string>> args, std::vector<std::unique_ptr<ASTNode>> body, std::shared_ptr<Environment> scope)
            :_name(name), _type(type), _args(std::move(args)), _body(std::move(body)), _scope(scope)
        {
        }

        void ASTFunction::Print(std::ostream& stream) const
        {
            stream << "<Function>:\nName:" << _name << "\nBody:\n";
            for(const std::unique_ptr<ASTNode>& node : _body)
            {
                node->Print(stream);
                stream << "\n";
            }
        }

        extern llvm::AllocaInst* CreateAlloca(llvm::LLVMContext& context, std::shared_ptr<Type> type, llvm::Function* func, llvm::StringRef name);

        llvm::Value* ASTFunction::Generate(llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& module)
        {
            std::vector<llvm::Type*> argTypes;
            for(auto& arg : _args)
                argTypes.push_back(arg.first->GetLLVMType(context));

            llvm::FunctionType* functionType = llvm::FunctionType::get(_type->GetLLVMType(context), argTypes, false);
            llvm::Function* function = llvm::Function::Create(functionType, llvm::Function::ExternalLinkage, _name, module);

            unsigned int i = 0;
            for(llvm::Argument& arg : function->args())
                arg.setName(_args[i++].second);

            llvm::BasicBlock* entryBlock = llvm::BasicBlock::Create(context, _name, function);
            builder.SetInsertPoint(entryBlock);

            _scope->namedValues.clear();
            i = 0;
            for(llvm::Argument& arg : function->args())
            {
                llvm::AllocaInst* alloca = CreateAlloca(context, _args[i++].first, function, arg.getName());
                builder.CreateStore(&arg, alloca);
                _scope->namedValues[std::string(arg.getName())] = alloca;
            }

            for(std::unique_ptr<ASTNode>& node : _body)
            {
                if(node->GetNodeType() == ASTNodeType::Return)
                {
                    node->SetType(_type);
                    node->Generate(context, builder, module, _scope);
                    return function;
                }
                node->Generate(context, builder, module, _scope);
            }
            return function;
        }
    }
}