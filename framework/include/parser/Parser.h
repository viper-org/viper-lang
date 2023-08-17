// Copyright 2023 solar-mist

#ifndef VIPER_FRAMEWORK_PARSER_PARSER_H
#define VIPER_FRAMEWORK_PARSER_PARSER_H 1

#include "lexer/Token.h"

#include <vector>

namespace parsing
{
    class Parser
    {
    public:
        Parser(std::vector<lexing::Token>& tokens);

        lexing::Token current() const;
        lexing::Token consume();
        lexing::Token peek(int offset) const;

    private:
        std::vector<lexing::Token>& mTokens;
        int mPosition;
    };
}

#endif