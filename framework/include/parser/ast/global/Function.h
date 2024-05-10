// Copyright 2024 solar-mist

#ifndef VIPER_FRAMEWORK_PARSER_AST_GLOBAL_FUNCTION_H
#define VIPER_FRAMEWORK_PARSER_AST_GLOBAL_FUNCTION_H 1

#include "parser/ast/Node.h"

namespace parser
{
    struct FunctionArgument
    {
        std::string name;
        Type* type;
    };

    class Function : public ASTNode
    {
    public:
        Function(Type* returnType, std::vector<FunctionArgument> arguments, std::optional<std::string> struc, std::string_view name, std::vector<ASTNodePtr>&& body, Scope* scope);

        Type* getReturnType() const;

        vipir::Value* emit(vipir::IRBuilder& builder, vipir::Module& module, Scope* scope, diagnostic::Diagnostics& diag) override;

    private:
        Type* mReturnType;
        std::vector<FunctionArgument> mArguments;
        std::optional<std::string> mStruct;
        std::string mName;
        std::vector<ASTNodePtr> mBody;
        ScopePtr mScope;
    };
    using FunctionPtr = std::unique_ptr<Function>;
}

#endif // VIPER_FRAMEWORK_PARSER_AST_GLOBAL_FUNCTION_H
