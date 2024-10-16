// Copyright 2024 solar-mist

#include "lexer/SourceLocation.h"

namespace lexer
{
    SourceLocation::SourceLocation(std::string_view file, unsigned int line, unsigned int col, unsigned int position)
        : file(file)
        , line(line)
        , col(col)
        , position(position)
    {
    }
}