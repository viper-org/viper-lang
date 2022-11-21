#include <parsing/ast/expression/variable.hh>
#include <environment.hh>

namespace Parsing
{
    Variable::Variable(std::string name)
        :ASTNode(ASTNodeType::Variable), _name(name)
    {
    }

    void Variable::Print(std::ostream& stream, int indent) const
    {
        stream << std::string(indent, ' ') << "<Variable>: " << _name;
    }

    llvm::Value* Variable::Emit(llvm::LLVMContext&, llvm::Module&, llvm::IRBuilder<>& builder)
    {
        llvm::AllocaInst* alloca = namedValues.at(_name);

        return builder.CreateLoad(alloca->getAllocatedType(), alloca);
    }

    std::string Variable::GetName() const
    {
        return _name;
    }
}