// Copyright 2024 solar-mist

#ifndef VIPER_COMPILER_OPTIONS_H
#define VIPER_COMPILER_OPTIONS_H 1

#include <string>
#include <vector>

enum class OptionType
{
    WarningSpec,
    FlagSpec,
    InputFile,
};

struct Option
{
    OptionType type;
    std::string value;

    static std::vector<Option> ParseOptions(int argc, char** argv);
    static std::string GetInputFile(const std::vector<Option>& options);
};

#endif // VIPER_COMPILER_OPTIONS_H