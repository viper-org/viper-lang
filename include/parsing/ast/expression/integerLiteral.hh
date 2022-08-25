#ifndef VIPER_AST_INTEGER_HH
#define VIPER_AST_INTEGER_HH
#include <parsing/ast/astNode.hh>

class IntegerLiteral : public ASTNode
{
public:
    IntegerLiteral(long long value);

    void Print(std::ostream& stream, int indent) const override;
private:
    long long _value;
};

#endif