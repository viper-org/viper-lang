// Copyright 2024 solar-mist


#include "preprocessor/Preprocessor.h"

#include <cassert>
#include <cctype>
#include <format>
#include <fstream>
#include <iostream>
#include <sstream>

namespace preprocessor
{
    Preprocessor::Preprocessor(std::string text)
        : mText(std::move(text))
    {
    }

    void Preprocessor::preprocess()
    {
        doPreprocess();
    }

    const std::string& Preprocessor::getText() const
    {
        return mText;
    }

    void Preprocessor::doPreprocess()
    {
        int position = 0;
        for (auto c : mText)
        {
            if (c == '#')
            {
                position = parseDirective(position);
            }
            ++position;
        }

        for (auto& directive : mDirectives)
        {
            if (auto include = dynamic_cast<IncludeDirective*>(directive.get()))
            {
                std::ifstream file = std::ifstream(include->getPath().data());
                std::stringstream ss;
                ss << file.rdbuf();

                if (ss.str().find(std::format("#include \"{}\"", include->getPath())) != std::string::npos)
                { // TODO: Report error properly
                    std::cerr << "Recursive include of " << include->getPath() << "\n";
                    std::exit(1);
                }
                mText.erase(mText.begin() + include->getStart(), mText.begin() + include->getEnd());
                mText.insert(include->getStart(), ss.str());
            }
        }
        if (!mDirectives.empty())
        {
            mDirectives.clear();
            doPreprocess();
        }
    }

    int Preprocessor::parseDirective(int position)
    {
        int startPos = position;
        ++position; // #
        if (mText.substr(position).starts_with("include"))
        {
            position += 7;
            while(std::isspace(mText[position])) ++position;
            assert(mText[position++] == '"');
            std::string path;
            while(mText[position] != '"')
                path += mText[position++];
            position++;
            mDirectives.push_back(std::make_unique<IncludeDirective>(startPos, position, std::move(path)));
        }
        return position;
    }
}