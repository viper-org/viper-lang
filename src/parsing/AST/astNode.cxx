#include <parsing/AST/astNode.hxx>

namespace Quark
{
    namespace Parsing
    {
        std::ostream& operator<<(std::ostream& stream, const ASTNode& node)
        {
            node.Print(stream);
            return stream;
        }
    }
}