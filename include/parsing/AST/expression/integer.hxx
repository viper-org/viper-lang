#ifndef VIPER_PARSING_AST_INTEGER_HXX
#define VIPER_PARSING_AST_INTEGER_HXX
#include <parsing/AST/astNode.hxx>

namespace Viper
{
    namespace Parsing
    {
        class IntegerLiteral : public ASTNode
        {
        public:
            IntegerLiteral(int value);

            void Print(std::ostream& stream) const override;
        private:
            int _value;
        };
    }
}

#endif