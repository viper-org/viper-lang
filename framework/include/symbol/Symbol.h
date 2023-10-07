#ifndef VIPER_FRAMEWORK_SYMBOL_SYMBOL_H
#define VIPER_FRAMEWORK_SYMBOL_SYMBOL_H 1

#include <type/Type.h>

#include <map>
#include <string>

class Symbol
{
public:
    Symbol() = default;
    Symbol(bool isLocal, Type* type, const std::string& name);

    bool getIsLocal() const;
    Type* getType() const;
    std::string_view getName() const;

private:
    bool mIsLocal;
    Type* mType;
    std::string mName;
};
using SymbolTable = std::map<std::string, Symbol>;

#endif