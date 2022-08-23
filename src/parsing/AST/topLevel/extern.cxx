#include <parsing/AST/topLevel/extern.hxx>
#include <codegen/functionSymbol.hxx>

namespace Viper
{
    namespace Parsing
    {
        ExternFunction::ExternFunction(std::string name, std::shared_ptr<Type> type, std::vector<std::pair<std::shared_ptr<Type>, std::string>> args)
            :_name(name), _type(type), _args(std::move(args))
        {
        }

        void ExternFunction::Print(std::ostream& stream) const
        {
            stream << "<Extern-Function>:\nName: " << _name;
        }

        std::pair<llvm::Value*, std::unique_ptr<CodeGen::Symbol>> ExternFunction::Generate(llvm::LLVMContext& context, llvm::IRBuilder<>&, llvm::Module& module)
        {
            std::vector<llvm::Type*> llvmArgTypes;
            for(std::pair<std::shared_ptr<Type>, std::string>& arg : _args)
                llvmArgTypes.push_back(arg.first->GetLLVMType(context));
            
            llvm::FunctionType* functionType = llvm::FunctionType::get(_type->GetLLVMType(context), llvmArgTypes, false);

            llvm::Function* function = llvm::Function::Create(functionType, llvm::Function::ExternalLinkage, _name, module);

            unsigned int i = 0;
            for(llvm::Argument& arg : function->args())
                arg.setName(_args[i++].second);

            std::vector<std::shared_ptr<Type>> types;
            for(auto& arg : _args)
                types.push_back(arg.first);

            return std::make_pair(function, nullptr);;
        }
    }
}