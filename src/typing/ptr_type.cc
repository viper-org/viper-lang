#include <llvm/IR/DerivedTypes.h>
#include <memory>
#include <typing/ptr_type.hh>

quark_ptr_type::quark_ptr_type(std::shared_ptr<quark_type> base)
    :quark_type(*base), base(base)
{
    llvm_name = std::string(llvm_name) + "*";
}

llvm::Type* quark_ptr_type::get_type() const
{
    return llvm::PointerType::get(base->get_type(), 0);
}

unsigned int quark_ptr_type::get_size() const
{
    return 8;
}

std::string_view quark_ptr_type::get_name() const
{
    return llvm_name;
}