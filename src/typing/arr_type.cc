#include <typing/arr_type.hh>

quark_arr_type::quark_arr_type(std::shared_ptr<quark_type> base, unsigned int length)
    :quark_type(*base), base(base), length(length)
{
}

llvm::Type* quark_arr_type::get_type() const
{
    return llvm::ArrayType::get(base->get_type(), length);
}

unsigned int quark_arr_type::get_size() const
{
    return length * size;
}

std::string_view quark_arr_type::get_name() const
{
    return llvm_name;
}