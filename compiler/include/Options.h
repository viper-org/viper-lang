// Copyright 2024 solar-mist

#ifndef VIPER_COMPILER_OPTIONS_H
#define VIPER_COMPILER_OPTIONS_H 1

#include "diagnostic/Diagnostic.h"

#include "vipir/Module.h"

#include <string>
#include <vector>

enum class OptionType
{
    WarningSpec,
    FlagSpec,
    OptimizationLevelSpec,
    InputFile,
};

struct Option
{
    OptionType type;
    std::string value;

    static std::vector<Option> ParseOptions(int argc, char** argv);
    static std::string GetInputFile(const std::vector<Option>& options);

    static void ParseOptimizingFlags(const std::vector<Option>& options, vipir::Module& module, diagnostic::Diagnostics& diag);
};

#endif // VIPER_COMPILER_OPTIONS_H