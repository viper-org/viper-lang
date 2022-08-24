#include <compiler.hh>
#include <diagnostics.hh>
#include <lexing/lexer.hh>
#include <sstream>
#include <iostream>

compiler::compiler(outputType output_type, const std::string& input_file_name)
    :output_type(output_type), input_file_name(input_file_name)
{
    input_handle = std::ifstream(input_file_name);
    if(!input_handle.is_open())
        diagnostics::fatal_error("viper", input_file_name + ": No such file or directory");

    std::stringstream buf;
    buf << input_handle.rdbuf();
    contents = buf.str();

    input_handle.close();
}

void compiler::compile()
{
    lexer lex(contents);
    for(const token& tok : lex.lex())
    {
        std::cout << tok << std::endl;
    }
}