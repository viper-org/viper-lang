#include <parsing/AST/topLevel/function.hxx>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/BasicBlock.h>

namespace Viper
{
    namespace Parsing
    {
        ASTFunction::ASTFunction(std::string name, std::shared_ptr<Type> type, std::vector<std::unique_ptr<ASTNode>> body, std::shared_ptr<Environment> scope)
            :_name(name), _type(type), _body(std::move(body)), _scope(scope)
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

        llvm::Value* ASTFunction::Generate(llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& module)
        {
            llvm::FunctionType* functionType = llvm::FunctionType::get(_type->GetLLVMType(context), false);
            llvm::Function* function = llvm::Function::Create(functionType, llvm::Function::ExternalLinkage, _name, module);

            llvm::BasicBlock* entryBlock = llvm::BasicBlock::Create(context, _name, function);
            builder.SetInsertPoint(entryBlock);

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