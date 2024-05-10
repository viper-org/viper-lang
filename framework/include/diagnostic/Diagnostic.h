// Copyright 2024 solar-mist

#ifndef VIPER_FRAMEWORK_DIAGNOSTIC_DIAGNOSTIC_H
#define VIPER_FRAMEWORK_DIAGNOSTIC_DIAGNOSTIC_H 1

#include <string>

namespace lexing
{
    class SourceLocation;
}

namespace fmt
{
    constexpr std::string_view bold = "\x1b[1m";
    constexpr std::string_view red  = "\x1b[31m";
    constexpr std::string_view defaults = "\x1b[0m";
}

namespace diagnostic
{
    class Diagnostics
    {
    public:
        void setImported(bool imported);
        void setFileName(std::string fileName);
        void setErrorSender(std::string sender);
        void setText(std::string text);

        [[noreturn]] void fatalError(std::string_view message);

        [[noreturn]] void compilerError(lexing::SourceLocation start, lexing::SourceLocation end, std::string_view message);

    private:
        std::string mFileName;
        std::string mSender;
        std::string mText;
        bool mImported;

        int getLinePosition(int lineNumber);
    };
}

#endif // VIPER_FRAMEWORK_DIAGNOSTIC_DIAGNOSTIC_H