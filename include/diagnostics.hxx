#ifndef QUARK_DIAGNOSTICS_HXX
#define QUARK_DIAGNOSTICS_HXX
#include <string_view>

namespace Quark
{
    namespace Lexing
    {
        class Token;
    }

    namespace Diagnostics
    {
        namespace {
            [[maybe_unused]] extern std::string_view fileName;
        }
        void FatalError(std::string_view sender, std::string_view message);
        
        [[noreturn]] void CompilerError(const unsigned int lineNumber, const unsigned int colNumber, 
            std::string_view message,
            const char* errorBegin, const char* errorEnd,
            const char* lineBegin, const char* lineEnd);
    
        void setFileName(std::string_view newFileName);
    }
}

#endif