#include <Test.h>

#include <iostream>
#include <lexer/Lexer.h>
#include <lexer/Token.h>

#include <string>
#include <vector>

TEST(Identifiers, LexerTests)
{
    struct TestCase
    {
        std::string identifier;
        Lexing::Token expected;
    };

    std::vector<TestCase> testCases = {
        {"a",  Lexing::Token(Lexing::TokenType::Identifier, "a")},
        {"aa", Lexing::Token(Lexing::TokenType::Identifier, "aa")},
        {"a0", Lexing::Token(Lexing::TokenType::Identifier, "a0")},
        {"a_", Lexing::Token(Lexing::TokenType::Identifier, "a_")},
        {"_",  Lexing::Token(Lexing::TokenType::Identifier, "_")},
        {"_a", Lexing::Token(Lexing::TokenType::Identifier, "_a")},
        {"_0", Lexing::Token(Lexing::TokenType::Identifier, "_0")},
        {"__", Lexing::Token(Lexing::TokenType::Identifier, "__")},
    };

    for (const TestCase& testCase : testCases)
    {
        Lexing::Lexer lexer(testCase.identifier);

        std::vector<Lexing::Token> tokens = lexer.lex();

        REQUIRE(tokens.size() == 1);

        REQUIRE(testCase.expected == tokens[0]);
    }
}

TEST(Parentheses, LexerTests)
{
    struct TestCase
    {
        std::string identifier;
        Lexing::Token expected;
    };

    std::vector<TestCase> testCases = {
        {"(", Lexing::Token(Lexing::TokenType::LeftParen)},
        {")", Lexing::Token(Lexing::TokenType::RightParen)},
    };

    for (const TestCase& testCase : testCases)
    {
        Lexing::Lexer lexer(testCase.identifier);

        std::vector<Lexing::Token> tokens = lexer.lex();

        REQUIRE(tokens.size() == 1);

        REQUIRE(testCase.expected == tokens[0]);
    }
}

TEST(Brackets, LexerTests)
{
    struct TestCase
    {
        std::string identifier;
        Lexing::Token expected;
    };

    std::vector<TestCase> testCases = {
        {"{", Lexing::Token(Lexing::TokenType::LeftBracket)},
        {"}", Lexing::Token(Lexing::TokenType::RightBracket)},
    };

    for (const TestCase& testCase : testCases)
    {
        Lexing::Lexer lexer(testCase.identifier);

        std::vector<Lexing::Token> tokens = lexer.lex();

        REQUIRE(tokens.size() == 1);

        REQUIRE(testCase.expected == tokens[0]);
    }
}

TEST(Whitespaces, LexerTests)
{
    struct TestCase
    {
        std::string identifier;
    };

    std::vector<std::string> testCases = {
        " ",
        "\n",
        "\t",
        "\v",
        "\f",
        "\r",
    };

    for (const std::string& testCase : testCases)
    {
        Lexing::Lexer lexer(testCase);

        std::vector<Lexing::Token> tokens = lexer.lex();

        REQUIRE(tokens.size() == 0);
    }
}