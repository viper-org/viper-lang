#ifndef SKETCH_DIAGNOSTICS_HXX
#define SKETCH_DIAGNOSTICS_HXX
#include <string_view>

namespace Sketch
{
    namespace Diagnostics
    {
        void FatalError(std::string_view sender, std::string_view message);
    }
}

#endif