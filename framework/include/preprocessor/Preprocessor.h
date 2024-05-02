// Copyright 2024 solar-mist

#ifndef VIPER_FRAMEWORK_PREPROCESSOR_PREPROCESSOR_H
#define VIPER_FRAMEWORK_PREPROCESSOR_PREPROCESSOR_H 1

#include "preprocessor/Directive.h"

#include <memory>
#include <string>
#include <vector>

namespace preprocessor
{
    class Preprocessor
    {
    public:
        Preprocessor(std::string text);

        void preprocess();

        const std::string& getText() const;

    private:
        std::string mText;
        std::vector<std::unique_ptr<Directive> > mDirectives;

        void doPreprocess();

        int parseDirective(int position);
    };
}

#endif // VIPER_FRAMEWORK_PREPROCESSOR_PREPROCESSOR_H