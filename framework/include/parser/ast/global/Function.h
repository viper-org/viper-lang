// Copyright 2024 solar-mist

#ifndef VIPER_FRAMEWORK_PARSER_AST_GLOBAL_FUNCTION_H
#define VIPER_FRAMEWORK_PARSER_AST_GLOBAL_FUNCTION_H 1

#include "parser/ast/Node.h"

namespace parser
{
    class Function : public ASTNode
    {
    public:
        Function(std::string_view name, std::vector<ASTNodePtr>&& body);

        vipir::Value* emit(vipir::IRBuilder& builder, vipir::Module& module) override;

    private:
        std::string mName;
        std::vector<ASTNodePtr> mBody;
    };
    using FunctionPtr = std::unique_ptr<Function>;
}

#endif // VIPER_FRAMEWORK_PARSER_AST_GLOBAL_FUNCTION_H