#include <parsing/AST/statement/return.hxx>

namespace Viper
{
    namespace Parsing
    {
        ReturnStatement::ReturnStatement(std::unique_ptr<ASTNode> value)
            :_value(std::move(value))
        {
        }

        void ReturnStatement::Print(std::ostream& stream) const
        {
            stream << "<Return-Statement>:\nValue: ";
            if(_value)
                _value->Print(stream);
            else
                stream << "None";
        }
    }
}