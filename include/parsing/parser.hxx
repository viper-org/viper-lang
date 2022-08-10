#ifndef VIPER_PARSER_HXX
#define VIPER_PARSER_HXX
#include <lexing/token.hxx>
#include <parsing/ast.hxx>
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
            
            Lexing::Token Current() const;
            Lexing::Token Consume();
            Lexing::Token Peek(int offset = 1) const;
            std::string GetTokenText(Lexing::Token token) const;

            void ExpectToken(Lexing::TokenType tokenType);

            std::vector<std::unique_ptr<ASTTopLevel>> Parse();

            std::unique_ptr<ASTNode> ParseExpression();

            std::unique_ptr<ASTTopLevel> ParseFunction();
        private:
            std::string _text;
            std::vector<Lexing::Token> _tokens;
            unsigned int _position;
        };
    }
}

#endif