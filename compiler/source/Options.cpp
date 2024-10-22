// Copyright 2024 solar-mist

#include "Options.h"

#include <algorithm>

std::vector<Option> Option::ParseOptions(int argc, char **argv)
{
    std::vector<Option> options;
    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];
        if (arg.starts_with('-'))
        {
            switch (arg[1])
            {
                case 'W':
                    options.push_back({OptionType::WarningSpec, arg.substr(2)});
                    break;

                case 'f':
                    options.push_back({OptionType::FlagSpec, arg.substr(2)});
                    break;
            }
        }
        else
        {
            options.push_back({OptionType::InputFile, arg});
        }
    }
    return options;
}

std::string Option::GetInputFile(const std::vector<Option>& options)
{
    auto it = std::find_if(options.begin(), options.end(), [](const auto& option){
        return option.type == OptionType::InputFile;
    });
    if (it != options.end()) return it->value;
    return "";
}