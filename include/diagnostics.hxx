#ifndef QUARK_DIAGNOSTICS_HXX
#define QUARK_DIAGNOSTICS_HXX
#include <string_view>

namespace Quark
{
    namespace Diagnostics
    {
        void FatalError(std::string_view sender, std::string_view message);
    }
}

#endif