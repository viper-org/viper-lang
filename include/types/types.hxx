#ifndef VIPER_TYPES_HXX
#define VIPER_TYPES_HXX
#include <types/type.hxx>
#include <types/integerType.hxx>
#include <types/arrayType.hxx>
#include <map>

namespace Viper
{
    extern std::map<std::string_view, std::shared_ptr<Type>> types;
    void InitDefaultTypes();
}

#endif