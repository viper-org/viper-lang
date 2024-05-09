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

    class DefineDirective : public Directive
    {
    public:
        DefineDirective(int start, int end, std::string name, std::string value);

        std::string_view getName() const;
        std::string_view getValue() const;

    private:
        std::string mName;
        std::string mValue;
    };

    class IfDirective : public Directive
    {
    public:
        IfDirective(int start, int end, std::string condition, bool invert);

        std::string_view getCondition() const;
        bool getInvert() const;

    private:
        std::string mCondition;
        bool mInvert;
    };

    class EndifDirective : public Directive
    {
    public:
        EndifDirective(int start, int end);
    };
}

#endif // VIPER_FRAMEWORK_PREPROCESSOR_DIRECTIVE_H