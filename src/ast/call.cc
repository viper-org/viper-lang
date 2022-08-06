#include <ast/call.hh>
#include <globals.hh>

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

    std::vector<llvm::Value*> arg_values;
    for(unsigned int i = 0; i < args.size(); i++)
    {
        llvm::Value* value = args[i]->codegen(env);
        if(value->getType() != (callee_func->args().begin()+i)->getType())
            value = builder.CreateSExtOrTrunc(value, (callee_func->args().begin()+i)->getType());
        
        arg_values.push_back(value);
    }

    return builder.CreateCall(callee_func, arg_values);
}