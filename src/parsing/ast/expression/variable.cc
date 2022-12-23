#include <parsing/ast/expression/variable.hh>
#include <environment.hh>

namespace Parsing
{
    Variable::Variable(std::string name, std::shared_ptr<Type> type)
        :ASTNode(ASTNodeType::Variable), _name(name)
    {
        _type = type;
    }

    void Variable::Print(std::ostream& stream, int indent) const
    {
        stream << std::string(indent, ' ') << "<Variable>: " << _name;
    }

    llvm::Value* Variable::Emit(llvm::LLVMContext&, llvm::Module& mod, llvm::IRBuilder<>& builder, std::shared_ptr<Environment> scope)
    {
        llvm::Value* ptr = scope->FindNamedValue(_name);
        if(!ptr)
            ptr = mod.getNamedGlobal(_name);

        return builder.CreateLoad(_type->GetLLVMType(), ptr);
    }

    std::string Variable::GetName() const
    {
        return _name;
    }
}