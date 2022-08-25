#ifndef VIPER_PARSER_HH
#define VIPER_PARSER_HH
#include <lexing/token.hh>
#include <parsing/ast.hh>
#include <vector>

class Parser
{
public:
    Parser(const std::vector<Token>& tokens, const std::string& text);

    std::vector<std::unique_ptr<ASTTopLevel>> Parse();
private:
    std::string _text;
    std::vector<Token> _tokens;
    unsigned int _position;

    Token Current() const;
    Token Consume();
    Token Peek(const int offset) const;

    void ExpectToken(TokenType tokenType);
    [[noreturn]] void ParserError(std::string message);

    std::unique_ptr<ASTTopLevel> ParseTopLevel();
    std::unique_ptr<ASTTopLevel> ParseFunction();

    std::unique_ptr<ASTNode> ParseExpression();
    std::unique_ptr<ASTNode> ParsePrimary();

    std::unique_ptr<ASTNode> ParseIntegerLiteral();
    
    std::unique_ptr<ASTNode> ParseReturnStatement();
};

#endif