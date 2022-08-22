#ifndef VIPER_AST_TOPLEVEL_EXTERN_HXX
#define VIPER_AST_TOPLEVEL_EXTERN_HXX
#include <parsing/AST/topLevel.hxx>
#include <parsing/AST/astNode.hxx>
#include <environment.hxx>
#include <memory>
#include <vector>

namespace Viper
{
    namespace Parsing
    {
        class ExternFunction : public ASTTopLevel
        {
        public:
            ExternFunction(std::string name, std::shared_ptr<Type> type, std::vector<std::pair<std::shared_ptr<Type>, std::string>> args);

            void Print(std::ostream& stream) const override;

            llvm::Value* Generate(llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& module) override;
        private:
            std::string _name;
            std::shared_ptr<Type> _type;
            std::vector<std::pair<std::shared_ptr<Type>, std::string>> _args;
        };
    }
}

#endif