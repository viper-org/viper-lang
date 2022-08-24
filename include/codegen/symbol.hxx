#ifndef VIPER_CODEGEN_SYMBOL_HXX
#define VIPER_CODEGEN_SYMBOL_HXX
#include <string>
#include <vector>
#include <memory>

namespace Viper
{
    namespace CodeGen
    {
        class Symbol
        {
        public:
            virtual ~Symbol() {  }

            virtual std::string ToString() const = 0;

            static std::vector<std::unique_ptr<Symbol>> ParseSymbols(std::string_view file);
        };
    }
}

#endif