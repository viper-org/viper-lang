// Copyright 2023 solar-mist


#ifndef VIPER_FRAMEWORK_PARSER_AST_GLOBAL_EXTERN_FUNCTION_H
#define VIPER_FRAMEWORK_PARSER_AST_GLOBAL_EXTERN_FUNCTION_H 1

#include "parser/ast/Node.h"

#include <string>

namespace parsing
{
    class ExternFunction : public ASTNode
    {
    public:
        ExternFunction(Type* type, const std::string& name);

        Type* getReturnType() const;
        std::string_view getName() const;

        vipir::Value* emit(vipir::Builder& builder, vipir::Module& module) override;

    private:
        Type* mReturnType;
        std::string mName;
    };
    using ExternFunctionPtr = std::unique_ptr<ExternFunction>;
}

#endif