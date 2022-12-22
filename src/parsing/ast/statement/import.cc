#include "parsing/ast/astNode.hh"
#include <parsing/ast/statement/import.hh>

namespace Parsing
{
    ImportStatement::ImportStatement(const std::string& name, std::shared_ptr<Type> type, std::vector<std::pair<std::shared_ptr<Type>, std::string>> args, bool isExtension)
        :ASTNode(ASTNodeType::ImportStatement), _name(name), _args(args), _isExtension(isExtension)
    {
        _type = type;
    }

    void ImportStatement::Print(std::ostream& stream, int indent) const
    {
        stream << std::string(indent, ' ') << "<Import>:\n";
        stream << std::string(indent, ' ') << "Name: " << _name;
    }

    llvm::Value* ImportStatement::Emit(llvm::LLVMContext&, llvm::Module& mod, llvm::IRBuilder<>&, std::shared_ptr<Environment>)
    {
        std::vector<llvm::Type*> paramTypes;
        std::vector<std::shared_ptr<Type>> argTypes;
        for(std::pair<std::shared_ptr<Type>, std::string> param : _args)
        {
            paramTypes.push_back(param.first->GetLLVMType());
            argTypes.push_back(param.first);
        }

        std::string mangledName = MangleFunction({_name}, argTypes, _type, _isExtension);

        llvm::FunctionType* funcType = llvm::FunctionType::get(_type->GetLLVMType(), paramTypes, false);
        llvm::Function* func = llvm::Function::Create(funcType, llvm::GlobalValue::ExternalLinkage, mangledName, mod);

        unsigned int i = 0;
        for(llvm::Argument& arg : func->args())
            arg.setName(_args[i++].second);

        return func;
    }
}