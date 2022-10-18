#ifndef VIPER_AST_EXPRESSION_VARIABLE_HH
#define VIPER_AST_EXPRESSION_VARIABLE_HH
#include <parsing/ast/astNode.hh>

namespace Parsing
{
    class Variable : public ASTNode
    {
    public:
        Variable(std::string name, std::shared_ptr<Type> type);

        void Print(std::ostream& stream, int indent) const override;

        SSA::Value* Emit(SSA::Builder& builder) override;

        std::string GetName() const;
    private:
        std::string _name;
    };
}

#endif