#ifndef VIPER_AST_STATEMENT_IMPORT_HH
#define VIPER_AST_STATEMENT_IMPORT_HH
#include <parsing/ast/astNode.hh>

namespace Parsing
{
    class ImportStatement : public ASTNode
    {
    public:
        ImportStatement(const std::string& name, std::shared_ptr<Type> type, std::vector<std::pair<std::shared_ptr<Type>, std::string>> args, bool isExtension);

        void Print(std::ostream& stream, int indent) const override;
        
        llvm::Value* Emit(llvm::LLVMContext& ctx, llvm::Module& mod, llvm::IRBuilder<>& builder, std::shared_ptr<Environment> scope) override;
    private:
        std::string _name;
        std::vector<std::pair<std::shared_ptr<Type>, std::string>> _args;
        bool _isExtension;
    };
}

#endif