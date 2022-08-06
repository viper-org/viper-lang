#include <ast/call.hh>
#include <globals.hh>
#include <llvm/IR/Function.h>
#include <memory>

call_expr::call_expr(std::string callee, std::vector<std::unique_ptr<ast_expr>> args)
    :callee(callee), args(std::move(args))
{
}

void call_expr::print(std::ostream& stream) const
{
    stream << "<call-expression>:\ncallee: " << callee << "\nargs: ";
    for(const std::unique_ptr<ast_expr>& arg : args)
    {
        arg->print(stream);
        stream << "\n";
    }
}

expr_type call_expr::get_type() const
{
    return expr_type::CALL;
}

llvm::Value* call_expr::codegen(std::shared_ptr<scope> env) const
{
    llvm::Function* callee_func = module.getFunction(callee);

    if(!callee_func)
    {
        std::vector<llvm::Type*> arg_types;
        for(const std::unique_ptr<ast_expr>& arg : args)
            arg_types.push_back(arg->type.llvm_info.getter(ctx));
        llvm::FunctionType* func_type = llvm::FunctionType::get(llvm::Type::getVoidTy(ctx), arg_types, false);

        callee_func = llvm::Function::Create(func_type, llvm::Function::ExternalLinkage, callee, module);

        //unsigned int i = 0;
        //for(llvm::Argument& arg : func->args())
        //    arg.setName("");
    }

    std::vector<llvm::Value*> arg_values;
    for(unsigned int i = 0; i < args.size(); i++)
    {
        llvm::Value* value = args[i]->codegen(env);
        if(value->getType() != (callee_func->args().begin()+i)->getType())
            value = type_info::convert(value, (callee_func->args().begin()+i)->getType());
        
        arg_values.push_back(value);
    }

    return builder.CreateCall(callee_func, arg_values);
}