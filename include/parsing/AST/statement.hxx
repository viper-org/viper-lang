#ifndef QUARK_PARSING_STATEMENT_HXX
#define QUARK_PARSING_STATEMENT_HXX
#include <parsing/AST/astNode.hxx>

namespace Quark
{
    namespace Parsing
    {
        class ASTStatement : public ASTNode
        {
        public:
            virtual ~ASTStatement();
        };
    }
}

#endif