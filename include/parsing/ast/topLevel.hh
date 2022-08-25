#ifndef VIPER_AST_TOPLEVEL_HH
#define VIPER_AST_TOPLEVEL_HH
#include <ostream>

class ASTTopLevel
{
public:
    virtual ~ASTTopLevel() {  }

    virtual void Print(std::ostream& stream) const = 0;
};

#endif