#ifndef VIPER_AST_STATEMENT_VARIABLE_DELCARATION_HH
#define VIPER_AST_STATEMENT_VARIABLE_DELCARATION_HH
#include <parsing/ast/astNode.hh>
#include <string>
#include <optional>
#include <vector>
#include <memory>

namespace Parsing
{
    class VariableDeclaration : public ASTNode
    {
    public:
        VariableDeclaration(const std::string& name, std::shared_ptr<Type> type, std::unique_ptr<ASTNode> initVal, Environment* scope = nullptr, std::optional<std::vector<std::pair<std::shared_ptr<Type>, std::string>>> args = std::nullopt);

        void Print(std::ostream& stream, int indent) const override;

        bool IsFunction() const;
        
        SSA::Value* Emit(SSA::Builder& builder, Environment* scope, bool isStatement) override;
    private:
        std::string _name;
        std::unique_ptr<ASTNode> _initVal;
        Environment* _scope;
        std::optional<std::vector<std::pair<std::shared_ptr<Type>, std::string>>> _args;
    };
}

#endif