// Copyright 2024 solar-mist

#include "parser/ast/global/Function.h"

namespace parser
{
    Function::Function(std::string name, std::vector<ASTNodePtr> body)
        : mName(std::move(name))
        , mBody(std::move(body))
    {
    }
}