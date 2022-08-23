#include <parsing/AST/topLevel/function.hxx>
#include <codegen/functionSymbol.hxx>
#include <llvm/IR/Attributes.h>
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

        std::pair<llvm::Value*, std::unique_ptr<CodeGen::Symbol>> ASTFunction::Generate(llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& module)
        {
            std::vector<llvm::Type*> llvmArgTypes;
            for(auto& arg : _args)
                llvmArgTypes.push_back(arg.first->GetLLVMType(context));

            llvm::FunctionType* functionType = llvm::FunctionType::get(_type->GetLLVMType(context), llvmArgTypes, false);
            llvm::Function* function = llvm::Function::Create(functionType, llvm::Function::ExternalLinkage, _name, module);
            llvm::outs() << function->getName() << "\n";

            llvm::Attribute noinline  = llvm::Attribute::get(context, llvm::Attribute::AttrKind::NoInline);
            llvm::Attribute nounwind  = llvm::Attribute::get(context, llvm::Attribute::AttrKind::NoUnwind);
            llvm::Attribute optnone   = llvm::Attribute::get(context, llvm::Attribute::AttrKind::OptimizeNone);
            llvm::Attribute sspstrong = llvm::Attribute::get(context, llvm::Attribute::AttrKind::StackProtectStrong);
            llvm::Attribute uwtable   = llvm::Attribute::get(context, llvm::Attribute::AttrKind::UWTable);

            llvm::AttributeList attributes;
            attributes = attributes.addFnAttribute(context, noinline);
            attributes = attributes.addFnAttribute(context, nounwind);
            attributes = attributes.addFnAttribute(context, optnone);
            attributes = attributes.addFnAttribute(context, sspstrong);
            attributes = attributes.addFnAttribute(context, uwtable);
            function->setAttributes(attributes);

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

            std::vector<std::shared_ptr<Type>> types;
            for(auto& arg : _args)
                types.push_back(arg.first);

            for(std::unique_ptr<ASTNode>& node : _body)
            {
                if(node->GetNodeType() == ASTNodeType::Return)
                {
                    node->SetType(_type);
                    node->Generate(context, builder, module, _scope);
                    return std::make_pair(function, std::make_unique<CodeGen::FunctionSymbol>(_name, types, _type));
                }
                node->Generate(context, builder, module, _scope);
            }
            return std::make_pair(function, std::make_unique<CodeGen::FunctionSymbol>(_name, types, _type));
        }
    }
}