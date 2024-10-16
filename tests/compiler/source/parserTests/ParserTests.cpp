#include "Test.h"

#include "parser/Parser.h"

#define CREATE_PARSER_INTROSPECTOR(MemberName) auto Get##Parser##_##MemberName () { return &parser::Parser::MemberName; }
#define CREATE_PARSER_OBJECT_INTROSPECTOR_REF(NodeType, MemberName) auto& Get##NodeType##_##MemberName (parser:: NodeType* n) { return n-> MemberName; }

struct ASTNodeIntrospector
{
    CREATE_PARSER_INTROSPECTOR(parseType);
    CREATE_PARSER_INTROSPECTOR(parsePrimary);
    CREATE_PARSER_INTROSPECTOR(parseFunction);
    CREATE_PARSER_INTROSPECTOR(parseReturnStatement);
    CREATE_PARSER_INTROSPECTOR(parseVariableDeclaration);
    CREATE_PARSER_INTROSPECTOR(parseIfStatement);
    CREATE_PARSER_INTROSPECTOR(parseIntegerLiteral);
    CREATE_PARSER_INTROSPECTOR(parseVariableExpression);
    CREATE_PARSER_INTROSPECTOR(parseCallExpression);

    
    CREATE_PARSER_OBJECT_INTROSPECTOR_REF(ASTNode, mType);

    CREATE_PARSER_OBJECT_INTROSPECTOR_REF(Function, mName);
    CREATE_PARSER_OBJECT_INTROSPECTOR_REF(Function, mBody);

    CREATE_PARSER_OBJECT_INTROSPECTOR_REF(ReturnStatement, mReturnValue);

    CREATE_PARSER_OBJECT_INTROSPECTOR_REF(IntegerLiteral, mValue);
};

namespace ParserTests
{
    struct TokenConstructor
    {
        TokenConstructor(lexer::TokenType tokenType, std::string text = "")
            : tokenType(tokenType)
            , text(text)
        {
        }

        lexer::TokenType tokenType;
        std::string text;
    };

    struct TestCase
    {
        TestCase()
        {
            Type::Init();
        }

        std::vector<TokenConstructor> tokens;
        ASTNodeIntrospector introspector;

        TestCase& setTokens(const std::vector<TokenConstructor>& newTokens) { tokens = newTokens; return *this; }
    };
    using TestCases = std::vector<TestCase>;

    struct TestCaseResult
    {
        parser::ASTNodePtr ast;
        diagnostic::Diagnostics diag;
        Scope globalScope;
    };


    TestCaseResult RunTestCase(const TestCase& testCase, auto function)
    {
        std::vector<lexer::Token> tokens;
        for (const auto& tokenConstructor : testCase.tokens)
        {
            tokens.emplace_back(tokenConstructor.text, tokenConstructor.tokenType, lexer::SourceLocation(), lexer::SourceLocation());
        }
        tokens.emplace_back("", lexer::TokenType::EndOfFile, lexer::SourceLocation(), lexer::SourceLocation());

        diagnostic::Diagnostics diag; // TODO: Add fake diagnostics to pass in
        Scope scope(nullptr, "", true);
        parser::Parser parser(tokens, diag, &scope);

        auto f = std::bind(function, parser);
        return {f(), diag, scope};
    }

    TEST(Functions, ParserTests)
    {
        TestCase testCase = TestCase()
            .setTokens({
                TokenConstructor(lexer::TokenType::FuncKeyword),
                TokenConstructor(lexer::TokenType::Identifier, "name"),
                TokenConstructor(lexer::TokenType::LeftParen),
                TokenConstructor(lexer::TokenType::RightParen),
                TokenConstructor(lexer::TokenType::RightArrow),
                TokenConstructor(lexer::TokenType::TypeKeyword, "void"),
                TokenConstructor(lexer::TokenType::LeftBrace),
                TokenConstructor(lexer::TokenType::RightBrace),
            });

        auto result = RunTestCase(testCase, testCase.introspector.GetParser_parseFunction());

        auto& globalNode = result.ast;
        auto functionNode = dynamic_cast<parser::Function*>(globalNode.get());
        REQUIRE(functionNode != nullptr);

        auto name = testCase.introspector.GetFunction_mName(functionNode);
        REQUIRE(name == "name");

        auto type = testCase.introspector.GetASTNode_mType(functionNode);
        FunctionType* expectedType = FunctionType::Create(Type::Get("void"), {});
        REQUIRE(type == expectedType);

        auto& body = testCase.introspector.GetFunction_mBody(functionNode);
        REQUIRE(body.empty() == true);
    }

    TEST(IntegerLiterals, ParserTests)
    {
        TestCase testCase = TestCase()
            .setTokens({
                TokenConstructor(lexer::TokenType::IntegerLiteral, "12"),
            });

        auto result = RunTestCase(testCase, testCase.introspector.GetParser_parseIntegerLiteral());

        auto integerLiteral = dynamic_cast<parser::IntegerLiteral*>(result.ast.get());
        REQUIRE(integerLiteral != nullptr);
        
        auto integerLiteralValue = testCase.introspector.GetIntegerLiteral_mValue(integerLiteral);
        REQUIRE(integerLiteralValue == 12);
    }

    TEST(NoValueReturnStatement, ParserTests)
    {
        TestCase testCase = TestCase()
            .setTokens({
                TokenConstructor(lexer::TokenType::ReturnKeyword),
                TokenConstructor(lexer::TokenType::Semicolon),
            });

        auto result = RunTestCase(testCase, testCase.introspector.GetParser_parseReturnStatement());

        auto returnStatement = dynamic_cast<parser::ReturnStatement*>(result.ast.get());
        REQUIRE(returnStatement != nullptr);

        auto& returnValue = testCase.introspector.GetReturnStatement_mReturnValue(returnStatement);
        REQUIRE(returnValue == nullptr);
    }

    TEST(ValueReturnStatement, ParserTests)
    {
        TestCase testCase = TestCase()
            .setTokens({
                TokenConstructor(lexer::TokenType::ReturnKeyword),
                TokenConstructor(lexer::TokenType::IntegerLiteral, "5"),
            });

        auto result = RunTestCase(testCase, testCase.introspector.GetParser_parseReturnStatement());

        auto returnStatement = dynamic_cast<parser::ReturnStatement*>(result.ast.get());
        REQUIRE(returnStatement != nullptr);

        auto& returnValue = testCase.introspector.GetReturnStatement_mReturnValue(returnStatement);
        auto integerLiteral = dynamic_cast<parser::IntegerLiteral*>(returnValue.get());
        REQUIRE(integerLiteral != nullptr);
        
        auto integerLiteralValue = testCase.introspector.GetIntegerLiteral_mValue(integerLiteral);
        REQUIRE(integerLiteralValue == 5);
    }
}