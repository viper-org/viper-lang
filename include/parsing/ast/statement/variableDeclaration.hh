#ifndef VIPER_AST_STATEMENT_VARIABLE_DELCARATION_HH
#define VIPER_AST_STATEMENT_VARIABLE_DELCARATION_HH
#include <parsing/ast/astNode.hh>
#include <string>
#include <memory>

namespace Parsing
{
    class VariableDeclaration : public ASTNode
    {
    public:
        VariableDeclaration(const std::string& name, std::unique_ptr<ASTNode> initVal, std::shared_ptr<Type> type, bool isGlobalVar);

        void Print(std::ostream& stream, int indent) const override;

        bool IsGlobal() const;


        llvm::Value* Emit(llvm::LLVMContext& ctx, llvm::Module& mod, llvm::IRBuilder<>& builder, std::shared_ptr<Environment> scope) override;
    private:
        std::string _name;
        std::unique_ptr<ASTNode> _initVal;
        bool _isGlobalVar;
    };
}

#endif