#include <parsing/AST/statement/variableDeclaration.hxx>

namespace Viper
{
    namespace Parsing
    {
        llvm::AllocaInst* CreateAlloca(llvm::LLVMContext& context, llvm::Function* func, llvm::StringRef name)
        {
            llvm::IRBuilder<> tmp_builder(&func->getEntryBlock(), func->getEntryBlock().begin());
            return tmp_builder.CreateAlloca(llvm::Type::getInt32Ty(context), nullptr, name);
        }

        VariableDeclaration::VariableDeclaration(std::string name, std::unique_ptr<ASTNode> value)
            :_name(name), _value(std::move(value))
        {
        }

        void VariableDeclaration::Print(std::ostream& stream) const
        {
            stream << "<Variable-Declaration>:\nName: " << _name << "\nValue: ";
            _value->Print(stream);
        }

        llvm::Value* VariableDeclaration::Generate(llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& module, std::shared_ptr<Environment> scope)
        {
            llvm::Function* function = builder.GetInsertBlock()->getParent();

            llvm::AllocaInst* alloca = CreateAlloca(context, function, _name);
            scope->namedValues[_name] = alloca;
            
            if(_value)
            {
                llvm::Value* initValue = _value->Generate(context, builder, module, scope);
                // TODO: Convert types

                return builder.CreateStore(initValue, alloca);
            }

            return nullptr;
        }
    }
}