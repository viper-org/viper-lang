#ifndef VIPER_NAMEMANGLING_H
#define VIPER_NAMEMANGLING_H

#include <string>
#include <vector>
#include "type/Type.h"

namespace symbol
{
    std::string mangleFunctionName(std::vector<std::string_view> names, std::vector<Type*> arguments);
}

#endif //VIPER_NAMEMANGLING_H
