// Copyright 2024 solar-mist

#include "parser/ast/expression/IntegerLiteral.h"

namespace parser
{
    IntegerLiteral::IntegerLiteral(std::uintmax_t value)
        : mValue(value)
    {
    }
}