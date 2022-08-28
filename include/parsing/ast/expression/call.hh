#ifndef VIPER_AST_CALL_HH
#define VIPER_AST_CALL_HH
#include <parsing/ast/astNode.hh>
#include <memory>

class CallExpr : public ASTNode // TODO: Implement args
{
public:
    CallExpr(const std::string& callee, std::vector<std::unique_ptr<ASTNode>> args);

    void Print(std::ostream& stream, int indent) const override;

    Codegen::Value* Generate(Codegen::Module& module, Codegen::Builder& builder) override;

    std::string GetName() const;
private:
    std::string _callee;
    std::vector<std::unique_ptr<ASTNode>> _args;
};

#endif