// Copyright 2024 solar-mist


#include "preprocessor/Preprocessor.h"
#include "preprocessor/Directive.h"

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
            else if (auto define = dynamic_cast<DefineDirective*>(directive.get()))
            {
                mText.erase(mText.begin() + define->getStart(), mText.begin() + define->getEnd());

                while (true)
                {
                    std::size_t position = mText.find(define->getName(), define->getStart());
                    if (position == std::string::npos) break;

                    mText.erase(mText.begin() + position, mText.begin() + position + define->getName().length());
                    mText.insert(position, define->getValue());
                }
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
            while (std::isspace(mText[position])) ++position;
            assert(mText[position++] == '"');
            std::string path;
            while(mText[position] != '"')
                path += mText[position++];
            position++;
            mDirectives.push_back(std::make_unique<IncludeDirective>(startPos, position, std::move(path)));
        }
        else if (mText.substr(position).starts_with(("define")))
        {
            position += 7;
            while (std::isspace(mText[position])) ++position;

            std::string name;
            while (!std::isspace(mText[position]))
                name += mText[position++];
            while (std::isspace(mText[position])) ++position;

            std::string value;
            while (mText[position] != '\n')
                value += mText[position++];
            position++;

            mDirectives.push_back(std::make_unique<DefineDirective>(startPos, position, std::move(name), std::move(value)));
        }
        return position;
    }
}