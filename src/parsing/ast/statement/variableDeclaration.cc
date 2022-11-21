#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <parsing/ast/statement/variableDeclaration.hh>

namespace Parsing
{
    VariableDeclaration::VariableDeclaration(const std::string& name, std::unique_ptr<ASTNode> initVal, bool isFunction)
        :ASTNode(ASTNodeType::VariableDeclaration), _name(name), _initVal(std::move(initVal)), _isFunction(isFunction)
    {
        _nodeType = (isFunction ? ASTNodeType::Function : ASTNodeType::VariableDeclaration);
    }

    void VariableDeclaration::Print(std::ostream& stream, int indent) const
    {
        stream << std::string(indent, ' ') << (_isFunction ? "<Function>:\n" : "<Variable-Declaration>:\n");
        stream << std::string(indent, ' ') << "Name: " << _name;
        if(_initVal)
        {
            stream << "\n" << std::string(indent, ' ') << "Value: \n";
            _initVal->Print(stream, indent + 2);
        }
    }

    llvm::Value* VariableDeclaration::Emit(llvm::LLVMContext& ctx, llvm::Module& mod, llvm::IRBuilder<>& builder)
    {
        if(_isFunction)
        {
            llvm::FunctionType* funcTy = llvm::FunctionType::get(builder.getInt32Ty(), {}, false);
            llvm::Function* func = llvm::Function::Create(funcTy, llvm::GlobalValue::ExternalLinkage, _name, mod);

            llvm::BasicBlock* bb = llvm::BasicBlock::Create(ctx, _name, func);
            builder.SetInsertPoint(bb);

            _initVal->Emit(ctx, mod, builder);

            return func;
        }
        throw; // Unreachable for now
    }
}