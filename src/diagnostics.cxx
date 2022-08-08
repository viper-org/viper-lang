#include <diagnostics.hxx>
#include <iostream>

constexpr std::string_view bold = "\x1b[1m";
constexpr std::string_view red  = "\x1b[31m";

constexpr std::string_view defaults = "\x1b[0m";

namespace Sketch
{
    namespace Diagnostics
    {
        void FatalError(std::string_view sender, std::string_view message)
        {
            std::cerr << bold << sender << ": " << red << "fatal error: " << defaults << message << "\n";
            std::cerr << "compilation terminated.\n";
            std::exit(1);
        }
    }
}