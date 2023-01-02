#ifndef VIPER_AST_STATEMENT_FUNCTION_HH
#define VIPER_AST_STATEMENT_FUNCTION_HH
#include <parsing/ast/astNode.hh>
#include <string>
#include <memory>

namespace Parsing
{
    class Function : public ASTNode
    {
    public:
        Function(const std::string& name, std::unique_ptr<ASTNode> initVal, std::shared_ptr<Environment> scope, std::shared_ptr<Type> returnType, std::vector<std::pair<std::shared_ptr<Type>, std::string>> params, bool isExtension);

        void Print(std::ostream& stream, int indent) const override;

        bool IsFunction() const;

        std::string_view GetMangledName() const;

        llvm::Value* Emit(llvm::LLVMContext& ctx, llvm::Module& mod, llvm::IRBuilder<>& builder, std::shared_ptr<Environment> scope) override;
    private:
        std::string _name;
        std::string _mangledName;
        std::unique_ptr<ASTNode> _initVal;
        std::shared_ptr<Environment> _scope;
        std::shared_ptr<Type> _returnType;
        std::vector<std::pair<std::shared_ptr<Type>, std::string>> _params;
        bool _isExtension;
    };
}

#endif