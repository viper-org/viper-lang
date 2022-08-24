#ifndef VIPER_DIAGNOSTICS_HH
#define VIPER_DIAGNOSTICS_HH
#include <string_view>

namespace diagnostics
{
    [[noreturn]] void fatal_error(std::string_view sender, std::string_view message);
    [[noreturn]] void error(std::string_view sender, std::string_view message);
    
    [[noreturn]] void compiler_error(const unsigned int line_number, const unsigned int col_number, 
        std::string_view message,
        const char* error_begin, const char* error_end,
        const char* line_begin, const char* line_end);

    void set_file_name(std::string_view new_file_name);
}

#endif