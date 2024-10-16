// Copyright 2024 solar-mist

#include "Test.h"

#include <lexer/Lexer.h>
#include <lexer/Token.h>

#include <climits>
#include <random>

namespace LexerTests
{
    struct TestCase
    {
        std::string inputText;
        std::vector<lexer::TokenType> expectedTokenTypes;
        std::vector<std::string> expectedTexts;

        TestCase& setInput(const std::string& newInput)                                       { inputText          = newInput;           return *this; }
        TestCase& setExpectedTokens(const std::vector<lexer::TokenType>& newExpectedTokens)   { expectedTokenTypes = newExpectedTokens;  return *this; }
        TestCase& setExpectedStrings(const std::vector<std::string>&     newExpectedStrings)  { expectedTexts      = newExpectedStrings; return *this; }
    };
    using TestCases = std::vector<TestCase>;

    void CheckTestCases(const TestCases& testCases)
    {
        for (const auto& testCase : testCases)
        {
            lexer::Lexer lexer(testCase.inputText, "");
            const auto tokens = lexer.lex();

            REQUIRE(tokens.size() == testCase.expectedTokenTypes.size() + 1); // eof

            for (size_t i = 0; i < tokens.size()-1; ++i)
            {
                REQUIRE(tokens[i].getTokenType() == testCase.expectedTokenTypes[i]);
                REQUIRE(tokens[i].getText() == testCase.expectedTexts[i]);
            }
            REQUIRE(tokens.back().getTokenType() == lexer::TokenType::EndOfFile);
        }
    }

    using random_bytes_engine = std::independent_bits_engine<
    std::default_random_engine, CHAR_BIT, unsigned char>;


    TEST(Keywords, LexerTests)
    {
        TestCases testCases =
        {
            TestCase()
                .setInput("func")
                .setExpectedTokens({lexer::TokenType::FuncKeyword})
                .setExpectedStrings({"func"}),

            TestCase()
                .setInput("return")
                .setExpectedTokens({lexer::TokenType::ReturnKeyword})
                .setExpectedStrings({"return"}),

            TestCase()
                .setInput("i8")
                .setExpectedTokens({lexer::TokenType::TypeKeyword})
                .setExpectedStrings({"i8"}),

            TestCase()
                .setInput("i16")
                .setExpectedTokens({lexer::TokenType::TypeKeyword})
                .setExpectedStrings({"i16"}),

            TestCase()
                .setInput("i32")
                .setExpectedTokens({lexer::TokenType::TypeKeyword})
                .setExpectedStrings({"i32"}),

            TestCase()
                .setInput("i64")
                .setExpectedTokens({lexer::TokenType::TypeKeyword})
                .setExpectedStrings({"i64"}),

            TestCase()
                .setInput("u8")
                .setExpectedTokens({lexer::TokenType::TypeKeyword})
                .setExpectedStrings({"u8"}),

            TestCase()
                .setInput("u16")
                .setExpectedTokens({lexer::TokenType::TypeKeyword})
                .setExpectedStrings({"u16"}),

            TestCase()
                .setInput("u32")
                .setExpectedTokens({lexer::TokenType::TypeKeyword})
                .setExpectedStrings({"u32"}),

            TestCase()
                .setInput("u64")
                .setExpectedTokens({lexer::TokenType::TypeKeyword})
                .setExpectedStrings({"u64"}),

            TestCase()
                .setInput("void")
                .setExpectedTokens({lexer::TokenType::TypeKeyword})
                .setExpectedStrings({"void"}),

            TestCase()
                .setInput("bool")
                .setExpectedTokens({lexer::TokenType::TypeKeyword})
                .setExpectedStrings({"bool"}),

            TestCase()
                .setInput("let")
                .setExpectedTokens({lexer::TokenType::LetKeyword})
                .setExpectedStrings({"let"}),

            TestCase()
                .setInput("if")
                .setExpectedTokens({lexer::TokenType::IfKeyword})
                .setExpectedStrings({"if"}),

            TestCase()
                .setInput("else")
                .setExpectedTokens({lexer::TokenType::ElseKeyword})
                .setExpectedStrings({"else"}),

            TestCase()
                .setInput("true")
                .setExpectedTokens({lexer::TokenType::TrueKeyword})
                .setExpectedStrings({"true"}),

            TestCase()
                .setInput("false")
                .setExpectedTokens({lexer::TokenType::FalseKeyword})
                .setExpectedStrings({"false"}),
        };

        CheckTestCases(testCases);
    }

    TEST(BinarySymbols, LexerTests)
    {
        TestCases testCases =
        {
            TestCase()
                .setInput("+")
                .setExpectedTokens({ lexer::TokenType::Plus })
                .setExpectedStrings({ "+" }),

            TestCase()
                .setInput("-")
                .setExpectedTokens({ lexer::TokenType::Minus })
                .setExpectedStrings({ "-" }),

            TestCase()
                .setInput("*")
                .setExpectedTokens({ lexer::TokenType::Star })
                .setExpectedStrings({ "*" }),

            TestCase()
                .setInput("/")
                .setExpectedTokens({ lexer::TokenType::Slash })
                .setExpectedStrings({ "/" }),
        };

        CheckTestCases(testCases);
    }

    TEST(ComparisonSymbols, LexerTests)
    {
        TestCases testCases =
        {
            TestCase()
                .setInput("==")
                .setExpectedTokens({ lexer::TokenType::DoubleEqual })
                .setExpectedStrings({ "==" }),

            TestCase()
                .setInput("!=")
                .setExpectedTokens({ lexer::TokenType::BangEqual })
                .setExpectedStrings({ "!=" }),

            TestCase()
                .setInput("<")
                .setExpectedTokens({ lexer::TokenType::LessThan })
                .setExpectedStrings({ "<" }),

            TestCase()
                .setInput(">")
                .setExpectedTokens({ lexer::TokenType::GreaterThan })
                .setExpectedStrings({ ">" }),

            TestCase()
                .setInput("<=")
                .setExpectedTokens({ lexer::TokenType::LessEqual })
                .setExpectedStrings({ "<=" }),

            TestCase()
                .setInput(">=")
                .setExpectedTokens({ lexer::TokenType::GreaterEqual })
                .setExpectedStrings({ ">=" }),
        };

        CheckTestCases(testCases);
    }

    TEST(MiscSymbols, LexerTests)
    {
        TestCases testCases =
        {
            TestCase()
                .setInput("->")
                .setExpectedTokens({ lexer::TokenType::RightArrow })
                .setExpectedStrings({ "->" }),

            TestCase()
                .setInput("(")
                .setExpectedTokens({ lexer::TokenType::LeftParen })
                .setExpectedStrings({ "(" }),

            TestCase()
                .setInput(")")
                .setExpectedTokens({ lexer::TokenType::RightParen })
                .setExpectedStrings({ ")" }),

            TestCase()
                .setInput("{")
                .setExpectedTokens({ lexer::TokenType::LeftBrace })
                .setExpectedStrings({ "{" }),

            TestCase()
                .setInput("}")
                .setExpectedTokens({ lexer::TokenType::RightBrace })
                .setExpectedStrings({ "}" }),

            TestCase()
                .setInput(";")
                .setExpectedTokens({ lexer::TokenType::Semicolon })
                .setExpectedStrings({ ";" }),

            TestCase()
                .setInput(":")
                .setExpectedTokens({ lexer::TokenType::Colon })
                .setExpectedStrings({ ":" }),

            TestCase()
                .setInput(",")
                .setExpectedTokens({ lexer::TokenType::Comma })
                .setExpectedStrings({ "," }),

            TestCase()
                .setInput("&")
                .setExpectedTokens({ lexer::TokenType::Ampersand })
                .setExpectedStrings({ "&" }),

            TestCase()
                .setInput("=")
                .setExpectedTokens({ lexer::TokenType::Equal })
                .setExpectedStrings({ "=" }),
        };

        CheckTestCases(testCases);
    }

    TEST(Literals, LexerTests)
    {
        TestCases testCases;

        random_bytes_engine rbe;
        unsigned int numbers[100];
        std::generate(std::begin(numbers), std::end(numbers), std::ref(rbe));
        for (auto num : numbers)
        {
            testCases.push_back(
                TestCase()
                    .setInput(std::to_string(num))
                    .setExpectedTokens({ lexer::TokenType::IntegerLiteral })
                    .setExpectedStrings({ std::to_string(num) })
            );
        }

        CheckTestCases(testCases);
    }

    TEST(Identifiers, LexerTests)
    {
        constexpr std::array chars = {
            'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 
            'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
            'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 
            'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
            '_',
            '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'
        };

        std::default_random_engine rng(std::random_device{}());
        std::uniform_int_distribution<> firstCharDist(0, chars.size() - 11); // first character cannot be a number
        std::uniform_int_distribution<> dist(0, chars.size() - 1);
        auto getRandomChar = [chars, &dist, &rng](){ return chars[dist(rng)]; };
        auto getRandomString = [getRandomChar, chars, &firstCharDist, &rng](size_t len){
            std::string ret(len, 0);
            ret.front() = chars[firstCharDist(rng)];
            std::generate_n(ret.begin()+1, len-1, getRandomChar);
            return ret;
        };

        std::uniform_int_distribution<> lengthDist(2, 1000);

        for (auto i = 0; i < 100; ++i)
        {
            auto length = lengthDist(rng);
            auto str = getRandomString(length);
            TestCase testCase = TestCase()
                .setInput(str)
                .setExpectedTokens({ lexer::TokenType::Identifier })
                .setExpectedStrings({ str });
            CheckTestCases({testCase});
        }
    }

    TEST(StrayCharacters, LexerTests)
    {
        TestCases testCases =
        {
            TestCase()
                .setInput("`")
                .setExpectedTokens({ lexer::TokenType::Error })
                .setExpectedStrings({ "`" }),

            TestCase()
                .setInput("!")
                .setExpectedTokens({ lexer::TokenType::Error })
                .setExpectedStrings({ "!" }),

            TestCase()
                .setInput("$")
                .setExpectedTokens({ lexer::TokenType::Error })
                .setExpectedStrings({ "$" }),

            TestCase()
                .setInput("%")
                .setExpectedTokens({ lexer::TokenType::Error })
                .setExpectedStrings({ "%" }),

            TestCase()
                .setInput("^")
                .setExpectedTokens({ lexer::TokenType::Error })
                .setExpectedStrings({ "^" }),

            TestCase()
                .setInput("[")
                .setExpectedTokens({ lexer::TokenType::Error })
                .setExpectedStrings({ "[" }),

            TestCase()
                .setInput("]")
                .setExpectedTokens({ lexer::TokenType::Error })
                .setExpectedStrings({ "]" }),

            TestCase()
                .setInput("'")
                .setExpectedTokens({ lexer::TokenType::Error })
                .setExpectedStrings({ "'" }),

            TestCase()
                .setInput("@")
                .setExpectedTokens({ lexer::TokenType::Error })
                .setExpectedStrings({ "@" }),

            TestCase()
                .setInput("#")
                .setExpectedTokens({ lexer::TokenType::Error })
                .setExpectedStrings({ "#" }),

            TestCase()
                .setInput("~")
                .setExpectedTokens({ lexer::TokenType::Error })
                .setExpectedStrings({ "~" }),

            TestCase()
                .setInput("\\")
                .setExpectedTokens({ lexer::TokenType::Error })
                .setExpectedStrings({ "\\" }),

            TestCase()
                .setInput("|")
                .setExpectedTokens({ lexer::TokenType::Error })
                .setExpectedStrings({ "|" }),

            TestCase()
                .setInput("?")
                .setExpectedTokens({ lexer::TokenType::Error })
                .setExpectedStrings({ "?" }),
        };

        CheckTestCases(testCases);
    }

    TEST(Whitespace, LexerTests)
    {
        TestCases testCases =
        {
            TestCase()
                .setInput(" ")
                .setExpectedTokens({})
                .setExpectedStrings({}),

            TestCase()
                .setInput("\t")
                .setExpectedTokens({})
                .setExpectedStrings({}),

            TestCase()
                .setInput("\n")
                .setExpectedTokens({})
                .setExpectedStrings({}),

            TestCase()
                .setInput("\v")
                .setExpectedTokens({})
                .setExpectedStrings({}),
        };

        CheckTestCases(testCases);
    }

    TEST(EdgeCases, LexerTests)
    {
        TestCases testCases =
        {
            TestCase()
                .setInput("= =")
                .setExpectedTokens({ lexer::TokenType::Equal, lexer::TokenType::Equal })
                .setExpectedStrings({ "=", "=" }),

            TestCase()
                .setInput("< =")
                .setExpectedTokens({ lexer::TokenType::LessThan, lexer::TokenType::Equal })
                .setExpectedStrings({ "<", "=" }),

            TestCase()
                .setInput("> =")
                .setExpectedTokens({ lexer::TokenType::GreaterThan, lexer::TokenType::Equal })
                .setExpectedStrings({ ">", "=" }),

            TestCase()
                .setInput("- >")
                .setExpectedTokens({ lexer::TokenType::Minus, lexer::TokenType::GreaterThan })
                .setExpectedStrings({ "-", ">" }),
        };

        CheckTestCases(testCases);
    }
}