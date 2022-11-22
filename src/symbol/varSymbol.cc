#include <symbol/varSymbol.hh>

VarSymbol::VarSymbol(const std::string& name, std::shared_ptr<Type> type)
    :_name(name), _type(type)
{
}

std::string_view VarSymbol::GetName() const
{
    return _name;
}

std::shared_ptr<Type> VarSymbol::GetType() const
{
    return _type;
}