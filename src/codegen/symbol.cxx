#include <codegen/symbol.hxx>
#include <lexing/lexer.hxx>
#include <parsing/parser.hxx>
#include <types/types.hxx>
#include <fstream>
#include <sstream>

namespace Viper
{
    namespace CodeGen
    {
        std::string GetTokenText(std::string text, Lexing::Token token)
        {
            return text.substr(token.getStart(), token.getEnd() - token.getStart());
        }

        std::vector<std::unique_ptr<Symbol>> Symbol::ParseSymbols(std::string_view path)
        {
            std::ifstream file(path.data());
            std::stringstream buf;
            buf << file.rdbuf();
            std::string text = buf.str().substr(0, buf.str().find_first_of(0x0A));
            Lexing::Lexer lexer(text);
            llvm::LLVMContext tmp;
            Parsing::Parser parser(lexer.Lex(), text, {}, tmp);

            return parser.ParseSymbols();
        }
    }
}