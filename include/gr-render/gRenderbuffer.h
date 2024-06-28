#pragma once

#include "gCommon.h"

namespace grr {
    class gRenderbuffer {
    public:
        static u32 Create();

        static void Bind(u32 id);

        static void SetBufferStorage(RenderbufferType internalFormat, u32 width, u32 height);

        static void Unbind();

        static void Destoy(u32 id);

    private:
        static u32 s_current;

        static std::unordered_map<RenderbufferType, u32> s_renderbufferApi;

        friend class gFramebuffer;
    };
} // namespace grr
