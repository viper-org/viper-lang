#ifndef QUARK_DIAGNOSTICS_HXX
#define QUARK_DIAGNOSTICS_HXX
#include <string_view>

namespace Quark
{
    namespace Diagnostics
    {
        void FatalError(std::string_view sender, std::string_view message);
        
        [[noreturn]] void CompilerError(std::string_view fileName, const unsigned int lineNumber, const unsigned int colNumber, std::string_view message, const char* surroundBegin, const char* surroundEnd);
    }
}

#endif