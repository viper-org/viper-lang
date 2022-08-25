#include <parsing/parser.hh>
#include <diagnostics.hh>

Parser::Parser(const std::vector<Token>& tokens, const std::string& text)
    :_text(text), _tokens(tokens), _position(0)
{
}

Token Parser::Current() const
{
    return _tokens[_position];
}

Token Parser::Consume()
{
    return _tokens[_position++];
}

Token Parser::Peek(const int offset) const
{
    return _tokens[_position + offset];
}

void Parser::ExpectToken(TokenType tokenType)
{
    if(Current().GetType() != tokenType)
    {
        Token temp(tokenType, "", 0, 0, 0, 0);

        ParserError("Expected '" + temp.TypeAsString() + "', found " + Current().GetText());
    }
}

void Parser::ParserError(std::string message)
{
    unsigned int start = Current().GetStart();
    while(_text[start] != '\n')
        start--;
    unsigned int end = Current().GetEnd();
    while(_text[end] != '\n')
        end++;
    Diagnostics::CompilerError(Current().GetLine(), Current().GetCol(),
    message, &_text[Current().GetStart()], &_text[Current().GetEnd()],
              &_text[start], &_text[end]);
}

std::vector<std::unique_ptr<ASTTopLevel>> Parser::Parse()
{
    std::vector<std::unique_ptr<ASTTopLevel>> result;
    while(_position < _tokens.size())
    {
        result.push_back(ParseTopLevel());
    }
    return result;
}

std::unique_ptr<ASTTopLevel> Parser::ParseTopLevel()
{
    switch(Current().GetType())
    {
        case TokenType::Asperand:
            return ParseFunction();
        default:
            ParserError("Expected top-level expression, found '" + Current().GetText() + "'");
    }
}

std::unique_ptr<ASTTopLevel> Parser::ParseFunction()
{
    Consume();

    ExpectToken(TokenType::Identifier);
    std::string name = Consume().GetText();

    ExpectToken(TokenType::LeftParen);
    Consume();
    // TODO: Parse args
    ExpectToken(TokenType::RightParen);
    Consume();

    ExpectToken(TokenType::RightArrow);
    Consume();

    ExpectToken(TokenType::Type);
    Consume(); // TODO: Parse type

    ExpectToken(TokenType::LeftBracket);
    Consume();

    std::vector<std::unique_ptr<ASTNode>> body;
    while(Current().GetType() != TokenType::RightBracket)
    {
        body.push_back(ParseExpression());
        ExpectToken(TokenType::Semicolon);
        Consume();
    }
    Consume();

    return std::make_unique<ASTFunction>(name, std::move(body));
}

std::unique_ptr<ASTNode> Parser::ParseExpression()
{
    return ParsePrimary();
}

std::unique_ptr<ASTNode> Parser::ParsePrimary()
{
    switch(Current().GetType())
    {
        case TokenType::Integer:
            return ParseIntegerLiteral();
        case TokenType::Return:
            return ParseReturnStatement();
        default:
            ParserError("Expected primary expression, found '" + Current().GetText() + "'");
    }
}

std::unique_ptr<ASTNode> Parser::ParseIntegerLiteral()
{
    long long value = std::stoll(Consume().GetText());

    return std::make_unique<IntegerLiteral>(value);
}

std::unique_ptr<ASTNode> Parser::ParseReturnStatement()
{
    Consume();

    if(Current().GetType() == TokenType::Semicolon)
        return std::make_unique<ReturnStatement>(nullptr);

    return std::make_unique<ReturnStatement>(ParseExpression());
}