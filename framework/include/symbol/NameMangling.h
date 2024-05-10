#ifndef VIPER_FRAMEWORK_SYMBOL_NAME_MANGLING_H
#define VIPER_FRAMEWORK_SYMBOL_NAME_MANGLING_H

#include <string>
#include <vector>
#include "type/Type.h"

namespace symbol
{
    std::string mangleFunctionName(std::vector<std::string_view> names, std::vector<Type*> arguments);
}

#endif //VIPER_FRAMEWORK_SYMBOL_NAME_MANGLING_H