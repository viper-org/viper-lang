#include "ast/compound.hh"
#include "token.hh"
#include <cctype>
#include <memory>
#include <parser.hh>
#include <lexer.hh>
#include <typing/types.hh>
#include <iostream>

std::vector<token> parser::tokens;
unsigned int       parser::position = 0;

static type_info              current_return_type;
static std::shared_ptr<scope> current_env;

token parser::current()
{
    return tokens[position];
}

token parser::consume()
{
    return tokens[position++];
}

token parser::peek(int offset)
{
    return tokens[position + offset];
}

int parser::get_token_precedence(token tok)
{
    switch(tok.type)
    {
        case token_type::increment:
        case token_type::decrement:
            return 45;
        case token_type::star:
        case token_type::slash:
            return 40;
        case token_type::plus:
        case token_type::minus:
            return 35;
        case token_type::less_than:
        case token_type::greater_than:
        case token_type::less_equal:
        case token_type::greater_equal:
            return 30;
        case token_type::equal:
        case token_type::not_equal:
            return 25;
        case token_type::assignment:
        case token_type::increment_assign:
        case token_type::decrement_assign:
            return 20;
        default:
            return 0;
    }
}

void parser::expect_token(token_type T)
{
    if (current().type != T)
    {
        token temp(T, "", 0);
        std::cerr << "\u001b[31mUnexpected token: " << current() << ". Expected: " << temp.get_type() << "\u001b[0m" << std::endl;
        std::exit(1);
    }
}

std::vector<std::unique_ptr<function_ast>> parser::parse(std::string_view file)
{
    std::vector<std::unique_ptr<function_ast>> result;
    tokens = lexer().lex(file);
    while(current().type != token_type::eof)
    {
        result.push_back(parse_func());
    }
    return result;
}

std::unique_ptr<function_ast> parser::parse_func()
{
    expect_token(token_type::asperand);
    consume();

    std::string name = consume().text;

    expect_token(token_type::lparen);
    consume();
    
    std::vector<std::pair<type_info, std::string>> args;
    while(current().type != token_type::rparen)
    {
        type_info T = types.at(consume().text);
        args.push_back(std::make_pair(T, consume().text));
        if(current().type == token_type::rparen)
            break;
        expect_token(token_type::comma);
        consume();
    }
    consume();

    expect_token(token_type::right_arrow);
    consume();

    type_info type = types.at(consume().text);
    current_return_type = type;

    expect_token(token_type::lbracket);
    consume();

    std::shared_ptr<scope> env = std::make_shared<scope>();
    current_env = env;

    std::vector<std::unique_ptr<ast_expr>> body;
    while(current().type != token_type::rbracket)
    {
        body.push_back(parse_expr());
    }
    consume();
    return std::make_unique<function_ast>(name, type, std::move(body), args, env);
}

std::unique_ptr<ast_expr> parser::parse_expr()
{
    std::unique_ptr<ast_expr> lhs = parse_primary();
    return parse_bin_op(1, std::move(lhs));
}

std::unique_ptr<ast_expr> parser::parse_int_expr()
{
    int sign = 1;
    if(current().type == token_type::minus)
    {
        consume();
        sign = -1;
    }
    expect_token(token_type::integer);
    return std::make_unique<integer_expr>(std::stoi(consume().text) * sign);
}

std::unique_ptr<ast_expr> parser::parse_identifier_expr()
{
    if(peek(1).type == token_type::lparen)
        return parse_call_expr();
    else if(peek(1).type == token_type::colon)
    {
        std::string label = consume().text;
        consume();
        return std::make_unique<label_expr>(label);
    }
    else
        return parse_var_expr();
}

std::unique_ptr<ast_expr> parser::parse_call_expr()
{
    std::string name = consume().text;

    expect_token(token_type::lparen);
    consume();

    std::vector<std::unique_ptr<ast_expr>> args;

    while(current().type != token_type::rparen)
    {
        args.push_back(parse_expr());
        if(current().type == token_type::rparen)
            break;
        expect_token(token_type::comma);
        consume();
    }

    consume();

    return std::make_unique<call_expr>(name, std::move(args));
}

std::unique_ptr<ast_expr> parser::parse_var_expr()
{
    std::unique_ptr<ast_expr> expr = std::make_unique<var_expr>(consume().text);
    return expr;
}

std::unique_ptr<ast_expr> parser::parse_var_assign()
{
    type_info T = types.at(consume().text);

    std::string name = consume().text;

    expect_token(token_type::assignment);
    consume();

    std::unique_ptr<ast_expr> value = parse_expr();
    return std::make_unique<var_decl>(T, name, std::move(value));
}

std::unique_ptr<ast_expr> parser::parse_paren_expr()
{
    expect_token(token_type::lparen);
    consume();
    std::unique_ptr<ast_expr> expr = parse_expr();
    expect_token(token_type::rparen);
    consume();
    return expr;
}

std::unique_ptr<ast_expr> parser::parse_compound_expr()
{
    expect_token(token_type::lbracket);
    consume();

    std::shared_ptr<scope> env = std::make_shared<scope>();
    env->outer = current_env;
    current_env = env;

    std::vector<std::unique_ptr<ast_expr>> expressions;

    while(current().type != token_type::rbracket)
        expressions.push_back(parse_expr());
    consume();

    current_env = env->outer;

    return std::make_unique<compound_expr>(std::move(expressions), env);
}

std::unique_ptr<ast_expr> parser::parse_keyword_expr()
{
    expect_token(token_type::keyword);
    if(current().text == "return")
    {
        consume();
        std::unique_ptr<ast_expr> value = parse_expr();
        value->type = current_return_type;
        return std::make_unique<return_stmt>(std::move(value));
    }
    else if(current().text == "if")
    {
        consume();

        expect_token(token_type::lparen);
        consume();
        std::unique_ptr<ast_expr> cond = parse_expr();
        expect_token(token_type::rparen);
        consume();

        std::shared_ptr<scope> env = std::make_shared<scope>();
        env->outer = current_env;
        current_env = env;

        std::unique_ptr<ast_expr> body = parse_expr();

        current_env = env->outer;

        if(current().text != "else")
            return std::make_unique<if_expr>(std::move(cond), std::move(body), nullptr, env, nullptr);
        consume();

        std::shared_ptr<scope> else_env = std::make_shared<scope>();
        else_env->outer = current_env;
        current_env = else_env;

        std::unique_ptr<ast_expr> else_body = parse_expr();

        current_env = else_env->outer;

        return std::make_unique<if_expr>(std::move(cond), std::move(body), std::move(else_body), env, else_env);
    }
    else if(current().text == "for")
    {
        consume();

        expect_token(token_type::lparen);
        consume();

        std::unique_ptr<ast_expr> init = parse_expr();

        expect_token(token_type::semicolon);
        consume();

        std::unique_ptr<ast_expr> cond = parse_expr();

        expect_token(token_type::semicolon);
        consume();

        std::unique_ptr<ast_expr> incr = parse_expr();

        expect_token(token_type::rparen);
        consume();

        std::shared_ptr<scope> env = std::make_shared<scope>();
        env->outer = current_env;
        current_env = env;

        std::unique_ptr<ast_expr> body = parse_expr();

        current_env = env->outer;

        return std::make_unique<for_expr>(std::move(init), std::move(cond), std::move(incr), env, std::move(body));

    }
    else if(current().text == "do")
    {
        consume();
        
        std::shared_ptr<scope> env = std::make_shared<scope>();
        env->outer = current_env;
        current_env = env;

        std::unique_ptr<ast_expr> body = parse_expr();

        current_env = env->outer;

        expect_token(token_type::keyword);
        if(consume().text != "while")
            return nullptr;
            // TODO: Throw error

        expect_token(token_type::lparen);
        consume();

        std::unique_ptr<ast_expr> cond = parse_expr();

        expect_token(token_type::rparen);
        consume();

        return std::make_unique<while_expr>(std::move(cond), std::move(body), env, true);
    }
    else if(current().text == "while")
    {
        consume();

        expect_token(token_type::lparen);
        consume();

        std::unique_ptr<ast_expr> cond = parse_expr();

        expect_token(token_type::rparen);
        consume();

        std::shared_ptr<scope> env = std::make_shared<scope>();
        env->outer = current_env;
        current_env = env;

        std::unique_ptr<ast_expr> body = parse_expr();

        current_env = env->outer;

        return std::make_unique<while_expr>(std::move(cond), std::move(body), env, false);
    }
    else if(current().text == "break")
    {
        consume();
        return std::make_unique<break_expr>();
    }
    else if(current().text == "goto")
    {
        consume();

        return std::make_unique<goto_expr>(consume().text);
    }
    else if(types.find(current().text) != types.end())
    {
        return parse_var_assign();
    }
    return nullptr;
}

std::unique_ptr<ast_expr> parser::parse_primary()
{
    switch(current().type)
    {
        case token_type::minus:
        case token_type::integer:
            return parse_int_expr();
        case token_type::lparen:
            return parse_paren_expr();
        case token_type::lbracket:
            return parse_compound_expr();
        case token_type::identifier:
        {
            return parse_identifier_expr();
        }
        case token_type::keyword:
            return parse_keyword_expr();
        default:
            std::cerr << "\u001b[31mUnexpected token: " << current() << ". Expected an expression\u001b[0m" << std::endl;
            std::exit(1);
    }
}

std::unique_ptr<ast_expr> parser::parse_bin_op(int expr_precedence, std::unique_ptr<ast_expr> lhs)
{
    while(true)
    {
        int token_precedence = get_token_precedence(current());
        if(token_precedence < expr_precedence)
            return lhs;
        
        token operand = consume();

        std::unique_ptr<ast_expr> rhs = parse_primary();

        int next_precedence = get_token_precedence(current());
        if(token_precedence < next_precedence)
            rhs = parse_bin_op(token_precedence + 1, std::move(rhs));

        lhs = std::make_unique<bin_op_expr>(operand, std::move(lhs), std::move(rhs));
    }
}