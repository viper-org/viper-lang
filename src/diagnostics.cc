#include <diagnostics.hh>
#include <iostream>

constexpr std::string_view bold = "\x1b[1m";
constexpr std::string_view red  = "\x1b[31m";

constexpr std::string_view defaults = "\x1b[0m";

namespace Diagnostics
{
    std::string_view fileName;

    void FatalError(std::string_view sender, std::string_view message)
    {
        std::cerr << bold << sender << ": " << red << "fatal error: " << defaults << message << "\n";
        std::cerr << "compilation terminated.\n";
        std::exit(1);
    }

    void Error(std::string_view sender, std::string_view message)
    {
        std::cerr << bold << sender << ": " << red << "error: " << defaults << message << "\n";
        std::cerr << "compilation terminated.\n";
        std::exit(1);
    }

    void CompilerError(const unsigned int lineNumber, const unsigned int colNumber, 
    std::string_view message,
    const char* errorBegin, const char* errorEnd,
    const char* lineBegin, const char* lineEnd)
    {
        std::string start  = std::string(lineBegin + 1, errorBegin);
        std::string error  = std::string(errorBegin, errorEnd);
        std::string end    = std::string(errorEnd, lineEnd);
        std::string spaces = std::string(start.length(), ' ');

        std::cerr << bold << fileName << ":" << lineNumber << ":" << colNumber << ": " << red << "error: " << defaults << message << "\n";
        std::cerr << "    " << lineNumber << " | " << start << bold << red << error << defaults << end << "\n";
        std::cerr << "      | " << spaces << bold << red << "^" << std::string(error.length() - 1, '~') << defaults << "\n";
        std::exit(1);
    }

    void setFileName(std::string_view newFileName)
    {
        fileName = newFileName;
    }
}