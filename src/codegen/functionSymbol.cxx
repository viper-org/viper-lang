#include <codegen/functionSymbol.hxx>

namespace Viper
{
    namespace CodeGen
    {
        FunctionSymbol::FunctionSymbol(std::string name, std::vector<std::shared_ptr<Type>> args, std::shared_ptr<Type> type)
            :_name(name), _args(args), _type(type)
        {
        }

        std::string FunctionSymbol::ToString() const
        {
            std::string result = "@" + _name + "(";
            if(_args.size())
            {
                for(unsigned int i = 0; i < _args.size() - 1; i++)
                {
                    result += _args[i]->GetName();
                    result += ",";
                }
            
                result += _args.back()->GetName();
            }
            result += ")";

            return result + _type->GetName().data();
        }

        std::string FunctionSymbol::GetName() const
        {
            return _name;
        }

        std::vector<std::shared_ptr<Type>> FunctionSymbol::GetArgs() const
        {
            return _args;
        }

        std::shared_ptr<Type> FunctionSymbol::GetType() const
        {
            return _type;
        }
    }
}