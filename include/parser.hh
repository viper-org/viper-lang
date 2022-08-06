#ifndef QRK_PARSER_HH
#define QRK_PARSER_HH
#include <ast.hh>
#include <token.hh>
#include <string>
#include <vector>

struct parser
{
    static std::vector<token> tokens;
    static unsigned int position;

    static token current();
    static token consume();
    static token peek(int);

    static int get_token_precedence(token);

    static void expect_token(token_type);

    static std::vector<std::unique_ptr<function_ast>> parse(std::string_view);

    static std::unique_ptr<function_ast> parse_func();

    static std::unique_ptr<ast_expr> parse_expr();

    static std::unique_ptr<ast_expr> parse_unary();

    static std::unique_ptr<ast_expr> parse_int_expr();
    
    static std::unique_ptr<ast_expr> parse_identifier_expr();
    static std::unique_ptr<ast_expr> parse_var_expr();
    static std::unique_ptr<ast_expr> parse_call_expr();

    static std::unique_ptr<ast_expr> parse_var_assign();

    static std::unique_ptr<ast_expr> parse_paren_expr();

    static std::unique_ptr<ast_expr> parse_compound_expr();

    static std::unique_ptr<ast_expr> parse_keyword_expr();

    static std::unique_ptr<ast_expr> parse_primary();
    static std::unique_ptr<ast_expr> parse_bin_op(int, std::unique_ptr<ast_expr>);
};

#endif