#include <parsing/AST/expression/variable.hxx>

namespace Viper
{
    namespace Parsing
    {
        Variable::Variable(std::string name)
            :_name(name)
        {
        }

        void Variable::Print(std::ostream& stream) const
        {
            stream << "<Variable>:\nName: " << _name;
        }

        llvm::Value* Variable::Generate(llvm::LLVMContext&, llvm::IRBuilder<>& builder, llvm::Module&, std::shared_ptr<Environment> scope)
        {
            llvm::AllocaInst* alloca = FindNamedValue(_name, scope);

            return builder.CreateLoad(alloca->getAllocatedType(), alloca, _name);
        }
    }
}