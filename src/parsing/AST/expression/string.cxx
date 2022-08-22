#include <parsing/AST/expression/string.hxx>

namespace Viper
{
    namespace Parsing
    {
        StringLiteral::StringLiteral(std::string text)
            :_text(text)
        {
            _type = std::make_unique<ArrayType>(text.length(), types.at("i8"));
        }

        void StringLiteral::Print(std::ostream& stream) const
        {
            stream << "<Integer-Literal>: " << _text;
        }

        llvm::Value* StringLiteral::Generate(llvm::LLVMContext&, llvm::IRBuilder<>& builder, llvm::Module&, std::shared_ptr<Environment>, std::vector<CodegenFlag>)
        {
            return builder.CreateGlobalStringPtr(_text, "string");
        }
    }
}