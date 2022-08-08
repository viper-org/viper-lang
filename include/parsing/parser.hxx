#ifndef QUARK_PARSER_HXX
#define QUARK_PARSER_HXX
#include <lexing/token.hxx>
#include <parsing/AST/astNode.hxx>
#include <memory>
#include <vector>

namespace Quark
{
    namespace Parsing
    {
        class Parser
        {
        public:
            Parser(const std::vector<Lexing::Token>& tokens);
            
            Lexing::Token Current() const;
            Lexing::Token Consume();
            Lexing::Token Peek(int offset = 1) const;

            std::vector<std::unique_ptr<ASTNode>> Parse();
        private:
            std::vector<Lexing::Token> _tokens;
            unsigned int _position;
        };
    }
}

#endif