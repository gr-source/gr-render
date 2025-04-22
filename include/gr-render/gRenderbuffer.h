#pragma once

#include "gCommon.h"

namespace grr {
    class gRenderbuffer {
    public:
        gRenderbuffer();
        ~gRenderbuffer();

    private:
        RenderbufferID renderbufferID;
    };
} // namespace grr
