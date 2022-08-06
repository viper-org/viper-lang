#ifndef QRK_PTR_TYPE_HH
#define QRK_PTR_TYPE_HH
#include <typing/type.hh>
#include <string>
#include <memory>
#include <unordered_map>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/ADT/Twine.h>
#include <globals.hh>

class quark_ptr_type : public quark_type
{
    std::shared_ptr<quark_type> base;
public:
    quark_ptr_type(std::shared_ptr<quark_type>);

    llvm::Type* get_type() const override;
    unsigned int get_size() const override;
    std::string_view get_name() const override;
};

#endif