#ifndef VIPER_AST_STATEMENT_IMPORT_HH
#define VIPER_AST_STATEMENT_IMPORT_HH
#include <parsing/ast/astNode.hh>
#include <string>
#include <optional>
#include <vector>
#include <memory>

namespace Parsing
{
    class ImportStatement : public ASTNode
    {
    public:
        ImportStatement(const std::string& name, std::shared_ptr<Type> type, std::vector<std::pair<std::shared_ptr<Type>, std::string>> args);

        void Print(std::ostream& stream, int indent) const override;
        
        SSA::Value* Emit(SSA::Builder& builder, Environment* scope, bool isStatement) override;
    private:
        std::string _name;
        std::vector<std::pair<std::shared_ptr<Type>, std::string>> _args;
    };
}

#endif