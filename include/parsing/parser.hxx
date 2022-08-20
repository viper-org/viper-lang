#ifndef VIPER_PARSER_HXX
#define VIPER_PARSER_HXX
#include <lexing/token.hxx>
#include <parsing/ast.hxx>
#include <environment.hxx>
#include <memory>
#include <vector>

namespace Viper
{
    namespace Parsing
    {
        class Parser
        {
        public:
            Parser(const std::vector<Lexing::Token>& tokens, const std::string& text);

            std::vector<std::unique_ptr<ASTTopLevel>> Parse();
        private:
            std::string _text;
            std::vector<Lexing::Token> _tokens;
            unsigned int _position;
            std::shared_ptr<Environment> _currentScope;

            Lexing::Token Current() const;
            Lexing::Token Consume();
            Lexing::Token Peek(int offset = 1) const;
            std::string GetTokenText(Lexing::Token token) const;
            int GetBinOpPrecedence(Lexing::TokenType tokenType) const;


            void ExpectToken(Lexing::TokenType tokenType);
            [[noreturn]] void ParserError(std::string message);


            std::unique_ptr<ASTTopLevel> ParseFunction();

            std::shared_ptr<Type> ParseType();

            std::unique_ptr<ASTNode> ParseExpression(int precedence = 1);
            std::unique_ptr<ASTNode> ParsePrimary();

            std::unique_ptr<ASTNode> ParseInteger();

            std::unique_ptr<ASTNode> ParseParenthesizedExpression();

            std::unique_ptr<ASTNode> ParseVariable();

            std::unique_ptr<ASTNode> ParseVariableDeclaration();

            std::unique_ptr<ASTNode> ParseIfStatement();

            std::unique_ptr<ASTNode> ParseWhileStatement();

            std::unique_ptr<ASTNode> ParseReturn();
        };
    }
}

#endif