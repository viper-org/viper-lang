// Copyright 2024 solar-mist

#ifndef VIPER_FRAMEWORK_PREPROCESSOR_DIRECTIVE_H
#define VIPER_FRAMEWORK_PREPROCESSOR_DIRECTIVE_H 1

#include <string>

namespace preprocessor
{
    class Directive
    {
    public:
        Directive(int start, int end) : mStart(start), mEnd(end) { }
        virtual ~Directive() { }

        int getStart() const { return mStart; }
        int getEnd() const { return mEnd; }

    protected:
        int mStart;
        int mEnd;
    };

    class IncludeDirective : public Directive
    {
    public:
        IncludeDirective(int start, int end, std::string path);

        std::string_view getPath() const;

    private:
        std::string mPath;
    };
}

#endif // VIPER_FRAMEWORK_PREPROCESSOR_DIRECTIVE_H