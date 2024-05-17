// Copyright 2024 solar-mist


#include "diagnostic/Diagnostic.h"

#include "lexer/Token.h"

#include <format>
#include <iostream>
#include <sstream>

namespace diagnostic
{
    void Diagnostics::setImported(bool imported)
    {
        mImported = imported;
    }

    void Diagnostics::setFileName(std::string fileName)
    {
        mFileName = std::move(fileName);
    }
    void Diagnostics::setErrorSender(std::string sender)
    {
        mSender = sender;
    }
    void Diagnostics::setText(std::string text)
    {
        mText = text;
    }


    void Diagnostics::fatalError(std::string_view message)
    {
        std::cerr << std::format("{}{}: {}fatal error: {}{}\n", fmt::bold, mSender, fmt::red, fmt::defaults, message);

        std::exit(EXIT_FAILURE);
    }

    void Diagnostics::compilerError(lexing::SourceLocation start, lexing::SourceLocation end, std::string_view message)
    {
        int lineStart = getLinePosition(start.line-1);
        int lineEnd = getLinePosition(end.line)-1;

        end.position += 1;
        std::string before = mText.substr(lineStart, start.position - lineStart);
        std::string error = mText.substr(start.position, end.position - start.position);
        std::string after = mText.substr(end.position, lineEnd - end.position);
        std::string spacesBefore = std::string(std::to_string(start.line).length(), ' ');
        std::string spacesAfter = std::string(before.length(), ' ');

        std::string imported = mImported ? " in imported file" : "";

        std::cerr << std::format("{}{}:{}:{} {}error{}: {}{}\n", fmt::bold, mFileName, start.line, start.column, fmt::red, imported, fmt::defaults, message);
        std::cerr << std::format("    {} | {}{}{}{}{}{}\n", start.line, before, fmt::bold, fmt::red, error, fmt::defaults, after);
        std::cerr << std::format("    {} | {}{}{}^{}{}\n", spacesBefore, spacesAfter, fmt::bold, fmt::red, std::string(error.length()-1, '~'), fmt::defaults);

        std::exit(EXIT_FAILURE);
    }

    void Diagnostics::compilerWarning(lexing::SourceLocation start, lexing::SourceLocation end, std::string_view message)
    {
        int lineStart = getLinePosition(start.line-1);
        int lineEnd = getLinePosition(end.line)-1;

        end.position += 1;
        std::string before = mText.substr(lineStart, start.position - lineStart);
        std::string error = mText.substr(start.position, end.position - start.position);
        std::string after = mText.substr(end.position, lineEnd - end.position);
        std::string spacesBefore = std::string(std::to_string(start.line).length(), ' ');
        std::string spacesAfter = std::string(before.length(), ' ');

        std::string imported = mImported ? " in imported file" : "";

        std::cerr << std::format("{}{}:{}:{} {}warning{}: {}{}\n", fmt::bold, mFileName, start.line, start.column, fmt::yellow, imported, fmt::defaults, message);
        std::cerr << std::format("    {} | {}{}{}{}{}{}\n", start.line, before, fmt::bold, fmt::yellow, error, fmt::defaults, after);
        std::cerr << std::format("    {} | {}{}{}^{}{}\n", spacesBefore, spacesAfter, fmt::bold, fmt::yellow, std::string(error.length()-1, '~'), fmt::defaults);
    }


    int Diagnostics::getLinePosition(int lineNumber)
    {
        int line = 0;
        for (int i = 0; i < lineNumber; ++i)
        {
            while(mText[line] != '\n')
            {
                ++line;
            }
            ++line;
        }
        return line;
    }
}