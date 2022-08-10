#ifndef VIPER_AST_TOPLEVEL_HXX
#define VIPER_AST_TOPLEVEL_HXX
#include <ostream>

namespace Viper
{
    namespace Parsing
    {
        class ASTTopLevel
        {
        public:
            virtual ~ASTTopLevel() {  }

            virtual void Print(std::ostream& stream) const = 0;
        };
    }
}

#endif