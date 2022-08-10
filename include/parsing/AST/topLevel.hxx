#ifndef QUARK_AST_TOPLEVEL_HXX
#define QUARK_AST_TOPLEVEL_HXX
#include <ostream>

namespace Quark
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