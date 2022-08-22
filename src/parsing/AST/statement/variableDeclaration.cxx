#include <parsing/AST/statement/variableDeclaration.hxx>

namespace Viper
{
    namespace Parsing
    {
        llvm::AllocaInst* CreateAlloca(llvm::LLVMContext& context, std::shared_ptr<Type> type, llvm::Function* func, llvm::StringRef name)
        {
            llvm::IRBuilder<> tmp_builder(&func->getEntryBlock(), func->getEntryBlock().begin());
            return tmp_builder.CreateAlloca(type->GetLLVMType(context), nullptr, name);
        }

        VariableDeclaration::VariableDeclaration(std::shared_ptr<Type> type, std::string name, std::unique_ptr<ASTNode> value)
            :_name(name), _value(std::move(value))
        {
            _nodeType = ASTNodeType::VariableDeclaration;
            _type = type;
        }

        void VariableDeclaration::Print(std::ostream& stream) const
        {
            stream << "<Variable-Declaration>:\nName: " << _name << "\nValue: ";
            _value->Print(stream);
        }

        llvm::Value* VariableDeclaration::Generate(llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& module, std::shared_ptr<Environment> scope, std::vector<CodegenFlag>)
        {
            llvm::Function* function = builder.GetInsertBlock()->getParent();

            llvm::AllocaInst* alloca = CreateAlloca(context, _type, function, _name);
            scope->namedValues[_name] = alloca;
            
            if(_value)
            {
                llvm::Value* initValue = _value->Generate(context, builder, module, scope);
                if(alloca->getType()->isPointerTy())
                {
                    if(alloca->getType()->getNonOpaquePointerElementType()->isArrayTy())
                    {
                        llvm::MaybeAlign align(1);
                        unsigned int size = alloca->getAllocatedType()->getArrayNumElements();
                        return builder.CreateMemCpy(alloca, align, initValue, align, size);
                    }
                }

                if(initValue->getType() != _type->GetLLVMType(context))
                    initValue = Type::Convert(initValue, _type->GetLLVMType(context), builder);

                return builder.CreateStore(initValue, alloca);
            }

            return nullptr;
        }
    }
}