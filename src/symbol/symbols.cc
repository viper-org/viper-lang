#include <symbol/symbols.hh>
#include <environment.hh>
#include <algorithm>

std::vector<VarSymbol*> varSymbols;

VarSymbol* FindSymbol(const std::string& name)
{
    auto it = std::find_if(varSymbols.begin(), varSymbols.end(), [&name](VarSymbol* var){
        return var->GetName() == name;
    });
    if(it != varSymbols.end())
        return *it;
    else
        return nullptr;
}