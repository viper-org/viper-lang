#include <parsing/AST/statement/break.hxx>

namespace Viper
{
    namespace Parsing
    {
        BreakStatement::BreakStatement()
        {
            _nodeType = ASTNodeType::BreakStatement;
        }

        void BreakStatement::Print(std::ostream& stream) const
        {
            stream << "<Break-Statement>";
        }

        llvm::Value* BreakStatement::Generate(llvm::LLVMContext&, llvm::IRBuilder<>& builder, llvm::Module&, std::shared_ptr<Environment> scope, std::vector<CodegenFlag>)
        {
            return builder.CreateBr(FindLabel("breakmerge", scope));
        }
    }
}