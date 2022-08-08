#ifndef QUARK_PARSING_ASTNODE_HXX
#define QUARK_PARSING_ASTNODE_HXX
#include <ostream>

namespace Quark
{
    namespace Parsing
    {
        class ASTNode
        {
        public:
            virtual ~ASTNode();

            virtual void Print(std::ostream& stream) const = 0;

            friend std::ostream& operator<<(std::ostream& stream, const ASTNode& node);
        };
    }
}

#endif