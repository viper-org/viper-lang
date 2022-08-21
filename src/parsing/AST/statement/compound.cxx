#include <parsing/AST/statement/compound.hxx>

namespace Viper
{
    namespace Parsing
    {
        CompoundStatement::CompoundStatement(std::vector<std::unique_ptr<ASTNode>> statements, std::shared_ptr<Environment> scope)
            :_statements(std::move(statements)), _scope(scope)
        {
            _nodeType = ASTNodeType::CompoundStatement;
        }

        void CompoundStatement::Print(std::ostream& stream) const
        {
            stream << "<Compound-Statement>:";
            for(const std::unique_ptr<ASTNode>& statement : _statements)
            {
                stream << "\nStatement: ";
                statement->Print(stream);
            }
        }

        llvm::Value* CompoundStatement::Generate(llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& module, std::shared_ptr<Environment>)
        {
            for(std::unique_ptr<ASTNode>& statement : _statements)
                statement->Generate(context, builder, module, _scope);
            
            return nullptr;
        }
    }
}