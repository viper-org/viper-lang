#ifndef VIPER_CODEGEN_FUNCTION_SYMBOL_HXX
#define VIPER_CODEGEN_FUNCTION_SYMBOL_HXX
#include <codegen/symbol.hxx>
#include <types/types.hxx>
#include <memory>

namespace Viper
{
    namespace CodeGen
    {
        class FunctionSymbol : public Symbol
        {
        public:
            FunctionSymbol(std::string name, std::vector<std::shared_ptr<Type>> args, std::shared_ptr<Type> type);

            std::string ToString() const override;

            std::string GetName() const;
            std::vector<std::shared_ptr<Type>> GetArgs() const;
            std::shared_ptr<Type> GetType() const;
        private:
            std::string _name;
            std::vector<std::shared_ptr<Type>> _args;
            std::shared_ptr<Type> _type;
        };
    }
}

#endif