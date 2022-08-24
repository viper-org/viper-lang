#include <parsing/AST/expression/call.hxx>
#include <diagnostics.hxx>

namespace Viper
{
    namespace Parsing
    {
        CallExpression::CallExpression(std::string callee, std::vector<std::unique_ptr<ASTNode>> args)
            :_callee(callee), _args(std::move(args))
        {
            _nodeType = ASTNodeType::CallExpression;
        }

        void CallExpression::Print(std::ostream& stream) const
        {
            stream << "<Call-Expression>:\nCallee: " << _callee << "\nArgs:";
            for(const std::unique_ptr<ASTNode>& arg : _args)
            {
                stream << "\n";
                arg->Print(stream);
            }
        }

        llvm::Value* CallExpression::Generate(llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& module, std::shared_ptr<Environment> scope, std::vector<CodegenFlag>)
        {
            llvm::Function* function = module.getFunction(_callee);

            if(!function)
            {
                Diagnostics::Error("viper", "Unknown function: '\x1b[1m" + _callee + "\x1b[0m'. Did you forget to import a library?");
            }

            std::vector<llvm::Value*> argValues;
            for(unsigned int i = 0; i < _args.size(); i++)
            {
                llvm::Value* value = _args[i]->Generate(context, builder, module, scope, {});
                if(value->getType() != (function->args().begin()+i)->getType())
                    value = Type::Convert(value, (function->args().begin()+i)->getType(), builder);
                
                argValues.push_back(value);
            }

            return builder.CreateCall(function, argValues);
        }
    }
}