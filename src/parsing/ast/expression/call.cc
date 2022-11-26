#include <iostream>
#include <parsing/ast/expression/call.hh>
#include <parsing/ast/expression/variable.hh>
#include <environment.hh>
#include <llvm/IR/DerivedTypes.h>

namespace Parsing
{
    CallExpr::CallExpr(std::unique_ptr<ASTNode> callee, std::vector<std::unique_ptr<ASTNode>> args)
        :ASTNode(ASTNodeType::Call), _callee(std::move(callee)), _args(std::move(args))
    {
    }

    void CallExpr::Print(std::ostream& stream, int indent) const
    {
        stream << std::string(indent, ' ') << "<Call>:\nCallee:\n";
        _callee->Print(stream, indent + 2);
    }

    llvm::Value* CallExpr::Emit(llvm::LLVMContext& ctx, llvm::Module& mod, llvm::IRBuilder<>& builder, std::shared_ptr<Environment> scope)
    {
        std::vector<llvm::Value*> argValues;
        unsigned int i = 0;
        for(std::unique_ptr<ASTNode>& arg : _args)
        {
            llvm::Value* value = arg->Emit(ctx, mod, builder, scope);
            argValues.push_back(value);
        }

        std::vector<llvm::Type*> argTypes;
        std::vector<std::shared_ptr<Type>> paramTypes;
        for(std::unique_ptr<ASTNode>& arg : _args)
        {
            argTypes.push_back(arg->GetType()->GetLLVMType());
            paramTypes.push_back(arg->GetType());
        }

        llvm::Value* callee;
        llvm::Type* type;
        if(_callee->GetNodeType() == ASTNodeType::Variable)
        {
            std::string name = static_cast<Variable*>(_callee.get())->GetName();
            std::string mangledName = GetMangledFunction(name, paramTypes);
            llvm::Function* func = mod.getFunction(mangledName);
            std::cout << mangledName << std::endl;
            type = func->getReturnType();
            callee = func;
        }
        else
        {
            callee = _callee->Emit(ctx, mod, builder, scope);
            type = callee->getType();
        }

        _type = std::make_shared<Type>(type);

        llvm::FunctionType* funcTy = llvm::FunctionType::get(type, argTypes, false);

        i = 0;
        for(llvm::Value*& arg : argValues)
            arg = Type::Convert(arg, funcTy->getFunctionParamType(i), builder);

        return builder.CreateCall(funcTy, callee, argValues);
    }
}