// Copyright 2024 solar-mist

#include "Options.h"

#include <algorithm>
#include <unordered_map>

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

                case 'O':
                    options.push_back({OptionType::OptimizationLevelSpec, arg.substr(2)});
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

static std::unordered_map<std::string, vipir::Pass> passes = {
    { "dead-code-elimination", vipir::Pass::DeadCodeElimination },
    { "dce", vipir::Pass::DeadCodeElimination },
    { "peephole", vipir::Pass::PeepholeOptimization },
    { "constant-folding", vipir::Pass::ConstantFolding },
};
   
void Option::ParseOptimizingFlags(const std::vector<Option>& options, vipir::Module& module, diagnostic::Diagnostics& diag) 
{
    for (const auto& option : options)
    {
        if (option.type == OptionType::FlagSpec)
        {
            if (option.value.starts_with("no-"))
            {
                auto it = passes.find(option.value.substr(3));
                if (it != passes.end());
                    // TODO: module.removePass(it->second);
            }
            else
            {
                auto it = passes.find(option.value);
                if (it != passes.end())
                    module.addPass(it->second);
            }
        }
        else if (option.type == OptionType::OptimizationLevelSpec)
        {
            char level = option.value.empty() ? '1' : option.value[0];

            switch (level)
            {
                case '0':
                case 'g': // debug, shouldn't do anything
                    //TODO: module.removeAllPasses();
                    break;

                case '3':
                case '2':
                case 's': // size, should do everything except size increasing optimizations
                case '1':
                    module.addPass(vipir::Pass::PeepholeOptimization);
                    module.addPass(vipir::Pass::DeadCodeElimination);
                    module.addPass(vipir::Pass::ConstantFolding);
                    break;

                default:
                    diag.fatalError("argument to -O should be '0', '1', '2', '3', 'g' or 's'");
            }
        }
    }

}
