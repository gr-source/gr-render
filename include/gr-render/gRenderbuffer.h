#pragma once

#include "gCommon.h"

namespace gr
{
    class gRenderbuffer
    {
    public:
        gRenderbuffer();
        ~gRenderbuffer();

    private:
        RenderbufferID renderbufferID;
    };
} // namespace grr
