// Copyright 2023 solar-mist


#ifndef VIPER_FRAMEWORK_PARSER_AST_GLOBAL_FUNCTION_H
#define VIPER_FRAMEWORK_PARSER_AST_GLOBAL_FUNCTION_H 1

#include "parser/ast/Node.h"

#include <string>
#include <vector>

namespace parsing
{
    class Function : public ASTNode
    {
    public:
        Function(const std::string& name, std::vector<ASTNodePtr> body);

        std::string_view getName() const;
        const std::vector<ASTNodePtr>& getBody() const;

    private:
        std::string mName;
        std::vector<ASTNodePtr> mBody;
    };
    using FunctionPtr = std::unique_ptr<Function>;
}

#endif