// Copyright 2023 solar-mist


#ifndef VIPER_FRAMEWORK_PARSER_AST_GLOBAL_FUNCTION_H
#define VIPER_FRAMEWORK_PARSER_AST_GLOBAL_FUNCTION_H 1

#include "parser/ast/Node.h"
#include "parser/ast/global/FunctionArgument.h"

#include <string>
#include <vector>

namespace parsing
{
    class Function : public ASTNode
    {
    public:
        Function(Type* type, const std::string& name, std::vector<FunctionArgument> arguments, std::vector<ASTNodePtr> body);

        Type* getReturnType() const;
        std::string_view getName() const;
        const std::vector<FunctionArgument>& getArguments() const;
        const std::vector<ASTNodePtr>& getBody() const;

        vipir::Value* emit(vipir::Builder& builder, vipir::Module& module) override;

    private:
        Type* mReturnType;
        std::string mName;
        std::vector<FunctionArgument> mArguments;
        std::vector<ASTNodePtr> mBody;
    };
    using FunctionPtr = std::unique_ptr<Function>;
}

#endif