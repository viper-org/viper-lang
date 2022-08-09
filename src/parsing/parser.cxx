#include <parsing/parser.hxx>
#include <sstream>

namespace Quark
{
    namespace Parsing
    {
        Parser::Parser(const std::vector<Lexing::Token>& tokens)
            :_tokens(tokens), _position(0)
        {
        }

        Lexing::Token Parser::Current() const
        {
            return _tokens[_position];
        }

        Lexing::Token Parser::Consume()
        {
            return _tokens[_position++];
        }

        Lexing::Token Parser::Peek(int offset) const
        {
            return _tokens[_position + offset];
        }

        std::vector<std::unique_ptr<ASTNode>> Parser::Parse()
        {
            std::vector<std::unique_ptr<ASTNode>> nodes;
            // TODO: Parse
            return nodes;
        }
    }
}