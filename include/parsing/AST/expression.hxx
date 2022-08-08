#ifndef QUARK_PARSING_EXPRESSION_HXX
#define QUARK_PARSING_EXPRESSION_HXX
#include <parsing/AST/astNode.hxx>

namespace Quark
{
    namespace Parsing
    {
        class ASTExpression : public ASTNode
        {
        public:
            virtual ~ASTExpression();
        };
    }
}

#endif