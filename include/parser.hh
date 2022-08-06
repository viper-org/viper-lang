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

    static int get_bin_op_precedence(token);
    static int get_un_op_precedence(token);

    static void expect_token(token_type);

    static program parse(std::string_view);

    static function_ast parse_func();
    static extern_func parse_extern();

    static type_info parse_type();

    static std::unique_ptr<ast_expr> parse_expr(int = 1);

    static std::unique_ptr<ast_expr> parse_unary();

    static std::unique_ptr<ast_expr> parse_int_expr();
    static std::unique_ptr<ast_expr> parse_char_expr();
    
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