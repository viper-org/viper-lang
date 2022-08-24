#include <diagnostics.hh>
#include <iostream>

constexpr std::string_view bold = "\x1b[1m";
constexpr std::string_view red  = "\x1b[31m";

constexpr std::string_view defaults = "\x1b[0m";

namespace diagnostics
{
    std::string_view file_name;

    void fatal_error(std::string_view sender, std::string_view message)
    {
        std::cerr << bold << sender << ": " << red << "fatal error: " << defaults << message << "\n";
        std::cerr << "compilation terminated.\n";
        std::exit(1);
    }

    void error(std::string_view sender, std::string_view message)
    {
        std::cerr << bold << sender << ": " << red << "error: " << defaults << message << "\n";
        std::cerr << "compilation terminated.\n";
        std::exit(1);
    }

    void compiler_error(const unsigned int line_number, const unsigned int col_number, 
    std::string_view message,
    const char* error_begin, const char* error_end,
    const char* line_begin, const char* line_end)
    {
        std::string start  = std::string(line_begin + 1, error_begin);
        std::string error  = std::string(error_begin, error_end);
        std::string end    = std::string(error_end, line_end);
        std::string spaces = std::string(start.length(), ' ');

        std::cerr << bold << file_name << ":" << line_number << ":" << col_number << ": " << red << "error: " << defaults << message << "\n";
        std::cerr << "    " << line_number << " | " << start << bold << red << error << defaults << end << "\n";
        std::cerr << "      | " << spaces << bold << red << "^" << std::string(error.length() - 1, '~') << defaults << "\n";
        std::exit(1);
    }

    void set_file_name(std::string_view new_file_name)
    {
        file_name = new_file_name;
    }
}