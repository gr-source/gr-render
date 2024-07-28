#pragma once

#include "gCommon.h"

namespace grr {
    struct gError {
        static const std::string &GetError();

        static void Clean();
    };
    
} // namespace grr
