#ifndef VIPER_AST_STATEMENT_CLASS_DEFINITION_HH
#define VIPER_AST_STATEMENT_CLASS_DEFINITION_HH
#include <parsing/ast/astNode.hh>
#include <string>
#include <memory>

namespace Parsing
{
    enum class AccessLevel
    {
        Public,
        Protected,
        Private
    };

    struct ClassField
    {
        AccessLevel accessLevel;
        std::shared_ptr<Type> type;
        std::string name;
    };
    struct ClassMethod
    {
        AccessLevel accessLevel;
        std::shared_ptr<Type> returnType;
        std::string name;
        std::vector<std::pair<std::shared_ptr<Type>, std::string>> params;
        std::shared_ptr<Environment> scope;
        std::unique_ptr<ASTNode> body;
    };

    class ClassDefinition : public ASTNode
    {
    public:
        ClassDefinition(const std::string& name, const std::vector<ClassField>& fields, std::vector<ClassMethod> methods);

        void Print(std::ostream& stream, int indent) const override;

        llvm::Value* Emit(llvm::LLVMContext& ctx, llvm::Module& mod, llvm::IRBuilder<>& builder, std::shared_ptr<Environment> scope) override;
    private:
        std::string _name;
        std::vector<ClassField> _fields;
        std::vector<ClassMethod> _methods;
    };
}

#endif