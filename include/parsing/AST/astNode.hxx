#ifndef VIPER_PARSING_ASTNODE_HXX
#define VIPER_PARSING_ASTNODE_HXX
#include <ostream>

namespace Viper
{
    namespace Parsing
    {
        class ASTNode
        {
        public:
            virtual ~ASTNode() {  }

            virtual void Print(std::ostream& stream) const = 0;
        };
    }
}

#endif