// Copyright 2024 solar-mist

#ifndef VIPER_FRAMEWORK_SYMBOL_IDENTIFIER_H
#define VIPER_FRAMEWORK_SYMBOL_IDENTIFIER_H

#include <string>
#include <vector>
#include "type/Type.h"

namespace symbol
{
    void AddIdentifier(std::string mangledName, std::vector<std::string> names);

    std::vector<std::string> GetSymbol(std::vector<std::string> givenNames, std::vector<std::string> activeNames);
}

#endif //VIPER_FRAMEWORK_SYMBOL_IDENTIFIER_H