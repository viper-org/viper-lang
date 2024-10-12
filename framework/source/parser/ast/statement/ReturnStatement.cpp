// Copyright 2024 solar-mist

#include "parser/ast/statement/ReturnStatement.h"

namespace parser
{
    ReturnStatement::ReturnStatement(ASTNodePtr returnValue)
        : mReturnValue(std::move(returnValue))
    {
    }
}