// Copyright 2024 solar-mist

#ifndef VIPER_FRAMEWORK_LEXER_LEXER_H
#define VIPER_FRAMEWORK_LEXER_LEXER_H 1

#include <lexer/SourceLocation.h>

#include <istream>
#include <optional>
#include <string>
#include <vector>

namespace lexer
{
    class Token;

    class Lexer
    {
    public:
        Lexer(std::string text, std::string_view fileName);

        std::vector<Token> lex();

    private:
        std::string mText;
        std::string_view mFileName;

        SourceLocation mSourceLocation;
        unsigned int mPosition;


        char current();
        char consume();
        char peek(int offset);

        std::optional<Token> getNextToken();


        static inline bool isIdentifierStart(char c) { return std::isalpha(c) || c == '_'; }
        static inline bool isIdentifier(char c)      { return std::isalnum(c) || c == '_'; }
        static inline bool isDigitSeparator(char c)  { return c == '_'; }
    };
}

#endif // VIPER_FRAMEWORK_LEXER_LEXER_H