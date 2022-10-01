#ifndef VIPER_DIAGNOSTICS_HH
#define VIPER_DIAGNOSTICS_HH
#include <string_view>

namespace Diagnostics
{
    [[noreturn]] void FatalError(std::string_view sender, std::string_view message);
    [[noreturn]] void Error(std::string_view sender, std::string_view message);
    
    [[noreturn]] void CompilerError(const unsigned int lineNumber, const unsigned int colNumber, 
        std::string_view message,
        const char* errorBegin, const char* errorEnd,
        const char* lineBegin, const char* lineEnd);

    void setFileName(std::string_view newFileName);
}

#endif