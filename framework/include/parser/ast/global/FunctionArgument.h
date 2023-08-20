// Copyright 2023 solar-mist

#ifndef VIPER_FRAMEWORK_PARSER_AST_GLOBAL_FUNCTION_ARGUMENT_H
#define VIPER_FRAMEWORK_PARSER_AST_GLOBAL_FUNCTION_ARGUMENT_H 1

#include "type/Type.h"

#include <string>

namespace parsing
{
    class FunctionArgument
    {
    public:
        FunctionArgument(const std::string& name, Type* type);

        const std::string& getName() const;
        Type* getType() const;

    private:
        std::string mName;
        Type* mType;
    };
}

#endif