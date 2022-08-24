#include <parsing/AST/expression/variable.hxx>

namespace Viper
{
    namespace Parsing
    {
        Variable::Variable(std::string name)
            :_name(name)
        {
            _nodeType = ASTNodeType::Variable;
        }

        void Variable::Print(std::ostream& stream) const
        {
            stream << "<Variable>:\nName: " << _name;
        }

        llvm::Value* Variable::Generate(llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module&, std::shared_ptr<Environment> scope, std::vector<CodegenFlag> flags)
        {
            llvm::AllocaInst* alloca = FindNamedValue(_name, scope).first;

            if(alloca->getAllocatedType()->isArrayTy())
            {
                llvm::Value* zero = llvm::ConstantInt::get(types.at("i64")->GetLLVMType(context), 0);
                return builder.CreateInBoundsGEP(alloca->getAllocatedType(), alloca, { zero, zero }, _name);
            }

            if(std::find(flags.begin(), flags.end(), CodegenFlag::NoLoad) != flags.end())
                return alloca;
            
            return builder.CreateLoad(alloca->getAllocatedType(), alloca, _name);
        }

        std::string Variable::GetName() const
        {
            return _name;
        }
    }
}