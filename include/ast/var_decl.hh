#ifndef QRK_AST_VAR_DECL_HH
#define QRK_AST_VAR_DECL_HH
#include <ast/expr.hh>
#include <llvm/ADT/StringRef.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Instructions.h>

llvm::AllocaInst* create_alloca(llvm::Function*, llvm::StringRef, std::shared_ptr<quark_type>, llvm::Value* = nullptr);

class var_decl : public ast_expr
{
    std::string name;
    std::unique_ptr<ast_expr> value;
public:
    var_decl(std::shared_ptr<quark_type>, std::string, std::unique_ptr<ast_expr>);

    void print(std::ostream&) const override;
    expr_type get_type() const override;
    llvm::Value* codegen(std::shared_ptr<scope>) const override;
};

#endif