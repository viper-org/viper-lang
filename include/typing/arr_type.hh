#ifndef QRK_ARR_TYPE_HH
#define QRK_ARR_TYPE_HH
#include <typing/type.hh>

class quark_arr_type : public quark_type
{
    std::shared_ptr<quark_type> base;
    unsigned int length;
public:
    quark_arr_type(std::shared_ptr<quark_type>, unsigned int);

    llvm::Type* get_type() const override;
    unsigned int get_size() const override;
    std::string_view get_name() const override;
};

#endif