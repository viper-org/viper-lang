// Copyright 2024 solar-mist

#ifndef VIPER_FRAMEWORK_LEXER_SOURCE_LOCATION_H
#define VIPER_FRAMEWORK_LEXER_SOURCE_LOCATION_H 1

#include <string_view>

namespace lexer
{
    struct SourceLocation
    {
        std::string_view file;

        unsigned int line;
        unsigned int col;
    };
}

#endif // VIPER_FRAMEWORK_LEXER_SOURCE_LOCATION_H