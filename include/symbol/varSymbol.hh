#ifndef VIPER_VAR_SYMBOL_HH
#define VIPER_VAR_SYMBOL_HH
#include <type/types.hh>
#include <string>

class VarSymbol
{
public:
    VarSymbol(const std::string& name, std::shared_ptr<Type> type);

    std::string_view GetName() const;
    std::shared_ptr<Type> GetType() const;

private:
    std::string _name;
    std::shared_ptr<Type> _type;
};

#endif