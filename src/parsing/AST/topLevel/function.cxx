#include <parsing/AST/topLevel/function.hxx>

namespace Quark
{
    namespace Parsing
    {
        ASTFunction::ASTFunction(std::string name, std::vector<std::unique_ptr<ASTNode>> body)
            :_name(name), _body(std::move(body))
        {
        }

        void ASTFunction::Print(std::ostream& stream) const
        {
            stream << "<function>:\nname:" << _name << "\nbody:";
            for(const std::unique_ptr<ASTNode>& node : _body)
            {
                stream << "\n";
                node->Print(stream);
            }
        }
    }
}