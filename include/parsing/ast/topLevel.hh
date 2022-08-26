#ifndef VIPER_AST_TOPLEVEL_HH
#define VIPER_AST_TOPLEVEL_HH
#include <ostream>
#include <codegen/value/global/global.hh>
#include <codegen/module.hh>
#include <codegen/builder.hh>

class ASTTopLevel
{
public:
    virtual ~ASTTopLevel() {  }

    virtual void Print(std::ostream& stream) const = 0;

    virtual Codegen::Global* Generate(Codegen::Module& module, Codegen::Builder& builder) = 0;
};

#endif