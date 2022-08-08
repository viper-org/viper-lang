#include <diagnostics.hxx>
#include <iostream>

constexpr std::string_view bold = "\x1b[1m";
constexpr std::string_view red  = "\x1b[31m";

constexpr std::string_view defaults = "\x1b[0m";

namespace Quark
{
    namespace Diagnostics
    {
        void FatalError(std::string_view sender, std::string_view message)
        {
            std::cerr << bold << sender << ": " << red << "fatal error: " << defaults << message << "\n";
            std::cerr << "compilation terminated.\n";
            std::exit(1);
        }

        void CompilerError(std::string_view fileName, const unsigned int lineNumber, const unsigned int colNumber, std::string_view message, const char *surroundBegin, const char *surroundEnd)
        {
            std::cerr << bold << fileName << ":" << lineNumber << ":" << colNumber << ": " << red << "error: " << defaults << message << "\n";
            std::cerr << "    " << lineNumber << " |     " << bold << red << std::string(surroundBegin, surroundEnd) << defaults << "\n";
            std::cerr << "      |     " << red << "^" << std::string(surroundEnd - surroundBegin - 1, '~') << defaults << "\n";
            std::exit(1);
        }
    }
}